#include <pch.h>
#include "Terrain/TerrainAnalysis.h"
#include "Terrain/MapMath.h"
#include "Agent/AStarAgent.h"
#include "Terrain/MapLayer.h"
#include "Projects/ProjectThree.h"

#include <iostream>
#include <fstream>
#include <string>

bool ProjectThree::implemented_fog_of_war() const // extra credit
{
    return false;
}

namespace {
	const int dir[8][2] = { {0,-1}, {-1,-1}, {-1,0}, {-1,1}, {0,1}, {1,1}, {1,0}, {1,-1} };
	#define SQRT_2_F 1.41421356237f
	#define ANG_TO_RAD 0.01745329252f
	#define MASK_STRIGHT 85
	#define MASK_DIAGONAL 170
	#define MASK_ALL 255

	int mapHeight;
	int mapWidth;
	unsigned map = -1;

	bool visible_graph[1600][1600];
	unsigned char neighbor[40][40];
	int pre_row;
	int pre_col;
	int cell_count;
	float dist_square_vec;

	void update_visible_graph()
	{
		mapHeight = terrain->get_map_height();
		mapWidth = terrain->get_map_width();
		cell_count = mapHeight * mapWidth;
		map = terrain->currentMap;
		std::string s("Source\\Student\\Project_3\\test");
		s += std::to_string(map);
		std::fstream fs(s, std::fstream::in);
		std::cout << map << " " << s << std::endl;
		for (int i = 0; i < cell_count; ++i)
		{
			fs.read((char*)&visible_graph[i], sizeof(visible_graph[0]));
		}

		fs.close();
	}
}

void IniData()
{
	int pre_row = -1;
	int pre_col = -1;
	dist_square_vec = Vec3::DistanceSquared(terrain->get_world_position(0, 0), terrain->get_world_position(0, 1));
	if(map != terrain->currentMap)
		update_visible_graph();

	for (int i = 0; i < mapHeight; ++i)
	{
		for (int j = 0; j < mapWidth; ++j)
		{
			unsigned char bits = 0;
			if (!terrain->is_wall(i, j))
			{
				bool valid[9];
				int n_row;
				int n_col;

				for (int k = 0; k < 7; k += 2)
				{
					n_row = i + dir[k][0];
					n_col = j + dir[k][1];
					if (terrain->is_valid_grid_position(n_row, n_col) && !terrain->is_wall(n_row, n_col))
					{
						bits |= (1 << k);
						valid[k] = true;
					}
					else
						valid[k] = false;
				}
				valid[8] = valid[0];
				for (int k = 1; k < 8; k += 2)
				{
					n_row = i + dir[k][0];
					n_col = j + dir[k][1];
					if (valid[k - 1] && valid[k + 1] && !terrain->is_wall(n_row, n_col))
					{
						bits |= (1 << k);
					}
				}
			}
			neighbor[i][j] = bits;
		}
	}
	
}

float distance_to_closest_wall(int row, int col)
{
	/*
		Check the euclidean distance from the given cell to every other wall cell,
		with cells outside the map bounds treated as walls, and return the smallest
		distance.  Make use of the is_valid_grid_position and is_wall member
		functions in the global terrain to determine if a cell is within map bounds
		and a wall, respectively.
	*/

	static const int dir[5][2] = { {0,-1}, {-1,0}, {0,1}, {1,0}, {0,-1} };
	float min_d = std::numeric_limits<float>::max();
	int n = terrain->get_map_height();
	bool found = false;
	for (int i = 1; i < n; ++i) {
		int diagnal_count = i;
		for (int j = 0; j < 4; ++j)
		{
			int n_row = row + dir[j][0] * i;
			int n_col = col + dir[j][1] * i;
			if (!terrain->is_valid_grid_position(n_row, n_col) || terrain->is_wall(n_row, n_col))
			{
				min_d = static_cast<float>(i);
				found = true;
				break;
			}
			for (int k = 0; k < diagnal_count; ++k)
			{
				n_row += dir[j + 1][0];
				n_col += dir[j + 1][1];
				if (!terrain->is_valid_grid_position(n_row, n_col) || terrain->is_wall(n_row, n_col))
				{
					min_d = static_cast<float>(sqrt((n_row - row) * (n_row - row) + (n_col - col) * (n_col - col)));
					diagnal_count = k;
					found = true;
					break;
				}
			}
		}
		if (found)
			break;
	}

	return min_d; // REPLACE THIS
}

bool is_clear_path(int row0, int col0, int row1, int col1)
{
    /*
        Two cells (row0, col0) and (row1, col1) are visible to each other if a line
        between their centerpoints doesn't intersect the four boundary lines of every
        wall cell.  You should puff out the four boundary lines by a very tiny amount
        so that a diagonal line passing by the corner will intersect it.  Make use of the
        line_intersect helper function for the intersection test and the is_wall member
        function in the global terrain to determine if a cell is a wall or not.
    */

	#define OFF_SET 0.5000001f
	static const Vec2 walls[5]{ {-OFF_SET,-OFF_SET}, {-OFF_SET,OFF_SET}, {OFF_SET,OFF_SET}, {OFF_SET,-OFF_SET}, {-OFF_SET,-OFF_SET}};
	int n0 = row0, m0 = col0, n = row1, m = col1;
	if (n0 > n)
		std::swap(n0, n);
	if(m0 > m)
		std::swap(m0, m);
	Vec2 p0(static_cast<float>(row0), static_cast<float>(col0));
	Vec2 p1(static_cast<float>(row1), static_cast<float>(col1));
	for (int i = n0; i <= n; ++i)
	{
		for (int j = m0; j <= m; ++j)
		{
			if (terrain->is_wall(i, j))
			{
				Vec2 wall_center(static_cast<float>(i), static_cast<float>(j));
				for (int k = 0; k < 4; ++k)
				{
					if (line_intersect(p0, p1, wall_center + walls[k], wall_center + walls[k+1]))
						return false;
				}
			}
		}
	}

    return true; // REPLACE THIS
}

void analyze_openness(MapLayer<float> &layer)
{
    /*
        Mark every cell in the given layer with the value 1 / (d * d),
        where d is the distance to the closest wall or edge.  Make use of the
        distance_to_closest_wall helper function.  Walls should not be marked.
    */

	for (int i = 0; i < mapHeight; ++i)
	{
		for (int j = 0; j < mapWidth; ++j)
		{
			if (!terrain->is_wall(i, j))
			{
				float d = distance_to_closest_wall(i, j);
				layer.set_value(i, j, 1.0f / (d * d));
			}

		}
	}
}

void analyze_visibility(MapLayer<float> &layer)
{
    /*
        Mark every cell in the given layer with the number of cells that
        are visible to it, divided by 160 (a magic number that looks good).  Make sure
        to cap the value at 1.0 as well.

        Two cells are visible to each other if a line between their centerpoints doesn't
        intersect the four boundary lines of every wall cell.  Make use of the is_clear_path
        helper function.
    */

	if (map != terrain->currentMap)
		update_visible_graph();

	for (int i = 0; i < cell_count; ++i)
	{
		int row0 = i / mapWidth;
		int col0 = i % mapWidth;
		int count = 0;
		if (terrain->is_wall(row0, col0))
			continue;
		for (int j = 0; j < cell_count; ++j)
		{
			if (visible_graph[i][j])
				++count;
		}
		layer.set_value(row0, col0, std::min(static_cast<float>(count) / 160.0f, 1.0f));
	}
}

void analyze_visible_to_cell(MapLayer<float> &layer, int row, int col)
{
    /*
        For every cell in the given layer mark it with 1.0
        if it is visible to the given cell, 0.5 if it isn't visible but is next to a visible cell,
        or 0.0 otherwise.

        Two cells are visible to each other if a line between their centerpoints doesn't
        intersect the four boundary lines of every wall cell.  Make use of the is_clear_path
        helper function.
    */

	if (map != terrain->currentMap)
		update_visible_graph();

	static int rows[1600];
	static int cols[1600];

	int end = 0;
	int i = row * mapHeight + col;
	for (int j = 0; j < cell_count; ++j)
	{
		int row1 = j / mapWidth;
		int col1 = j % mapWidth;
		if (visible_graph[i][j])
		{
			layer.set_value(row1, col1, 1.0f);
			rows[end] = row1;
			cols[end] = col1;
			++end;
		}
		else
			layer.set_value(row1, col1, 0.0f);
	}

	for (int k = 0; k < end; ++k)
	{
		int i = rows[k];
		int j = cols[k];
		int n_row;
		int n_col;
		for (int k = 0; k < 8; ++k)
		{
			if (neighbor[i][j] & (1 << k))
			{
				n_row = i + dir[k][0];
				n_col = j + dir[k][1];
				if (layer.get_value(n_row, n_col) == 0.0f)
				{
					layer.set_value(n_row, n_col, 0.5f);
				}
			}
		}
	}
}

void analyze_agent_vision(MapLayer<float> &layer, const Agent *agent)
{
    /*
        For every cell in the given layer that is visible to the given agent,
        mark it as 1.0, otherwise don't change the cell's current value.

        You must consider the direction the agent is facing.  All of the agent data is
        in three dimensions, but to simplify you should operate in two dimensions, the XZ plane.

        Take the dot product between the view vector and the vector from the agent to the cell,
        both normalized, and compare the cosines directly instead of taking the arccosine to
        avoid introducing floating-point inaccuracy (larger cosine means smaller angle).

        Give the agent a field of view slighter larger than 180 degrees.

        Two cells are visible to each other if a line between their centerpoints doesn't
        intersect the four boundary lines of every wall cell.  Make use of the is_clear_path
        helper function.
    */

	if (map != terrain->currentMap)
		update_visible_graph();

	const auto & agent_v = agent->get_position();
	Vec3 agent_fv_norm = agent->get_forward_vector();
	agent_fv_norm.Normalize();
	Vec2 face(agent_fv_norm.x, agent_fv_norm.z);
	face.Normalize();
	GridPos agent_pos = terrain->get_grid_position(agent_v);
	static const float fov_cos = cosf(ANG_TO_RAD * 90.01f);
	int vi = agent_pos.row * mapHeight + agent_pos.col;

	for (int i = 0; i < mapHeight; ++i)
	{
		for (int j = 0; j < mapWidth; ++j)
		{
			if (!terrain->is_wall(i, j))
			{
				const auto & tile_v = terrain->get_world_position(i, j) - agent_v;
				Vec2 tile_2(tile_v.x, tile_v.z);
				tile_2.Normalize();
				if (face.Dot(tile_2) >= fov_cos && visible_graph[vi][i * mapHeight + j])
				{					
					layer.set_value(i, j, 1.0f);
				}
			}
		}
	}
}

void propagate_solo_occupancy(MapLayer<float> &layer, float decay, float growth)
{
    /*
        For every cell in the given layer:

            1) Get the value of each neighbor and apply decay factor
            2) Keep the highest value from step 1
            3) Linearly interpolate from the cell's current value to the value from step 2
               with the growing factor as a coefficient.  Make use of the lerp helper function.
            4) Store the value from step 3 in a temporary layer.
               A float[40][40] will suffice, no need to dynamically allocate or make a new MapLayer.

        After every cell has been processed into the temporary layer, write the temporary layer into
        the given layer;
    */
	
	static float temp[40][40];
	float exp_d1 = exp2f(-decay);
	float exp_d2 = exp2f(-SQRT_2_F * decay);
    
	for (int i = 0; i < mapHeight; ++i)
	{
		for (int j = 0; j < mapWidth; ++j)
		{
			if (terrain->is_wall(i, j))
				continue;
			float max_i = 0.0f;
			for (int k = 0; k < 8; k+=2)
			{
				if (neighbor[i][j] & (1 << k))
				{
					max_i = std::max(max_i, layer.get_value(i + dir[k][0], j + dir[k][1]) * exp_d1);
				}
			}
			for (int k = 1; k < 8; k += 2)
			{
				if (neighbor[i][j] & (1 << k))
				{
					max_i = std::max(max_i, layer.get_value(i + dir[k][0], j + dir[k][1]) * exp_d2);
				}
			}
			temp[i][j] = lerp(layer.get_value(i, j), max_i, growth);
		}
	}
	for (int i = 0; i < mapHeight; ++i)
	{
		for (int j = 0; j < mapWidth; ++j)
		{
			if (!terrain->is_wall(i, j))
				layer.set_value(i, j, temp[i][j]);
		}
	}
}

void propagate_dual_occupancy(MapLayer<float> &layer, float decay, float growth)
{
    /*
        Similar to the solo version, but the values range from -1.0 to 1.0, instead of 0.0 to 1.0

        For every cell in the given layer:

        1) Get the value of each neighbor and apply decay factor
        2) Keep the highest ABSOLUTE value from step 1
        3) Linearly interpolate from the cell's current value to the value from step 2
           with the growing factor as a coefficient.  Make use of the lerp helper function.
        4) Store the value from step 3 in a temporary layer.
           A float[40][40] will suffice, no need to dynamically allocate or make a new MapLayer.

        After every cell has been processed into the temporary layer, write the temporary layer into
        the given layer;
    */

	static float temp[40][40];
	float exp_d1 = exp2f(-decay);
	float exp_d2 = exp2f(-SQRT_2_F * decay);
	
	for (int i = 0; i < mapHeight; ++i)
	{
		for (int j = 0; j < mapWidth; ++j)
		{
			if (terrain->is_wall(i, j))
				continue;
			float max_i = 0.0f;
			float sign = 1.0f;
			for (int k = 0; k < 8; k += 2)
			{
				if (neighbor[i][j] & (1 << k))
				{
					float new_i = layer.get_value(i + dir[k][0], j + dir[k][1]) * exp_d1;
					float new_sign = 1.0f;
					if (new_i < 0)
						new_sign = -1.0;
					if (max_i < new_i * new_sign)
					{
						max_i = new_i * new_sign;
						sign = new_sign;
					}
				}
			}
			for (int k = 1; k < 8; k += 2)
			{
				if (neighbor[i][j] & (1 << k))
				{
					float new_i = layer.get_value(i + dir[k][0], j + dir[k][1]) * exp_d2;
					float new_sign = 1.0f;
					if (new_i < 0)
						new_sign = -1.0;
					if (max_i < new_i * new_sign)
					{
						max_i = new_i * new_sign;
						sign = new_sign;
					}
				}
			}
			temp[i][j] = lerp(layer.get_value(i, j), max_i * sign, growth);
		}
	}
	for (int i = 0; i < mapHeight; ++i)
	{
		for (int j = 0; j < mapWidth; ++j)
		{
			if (!terrain->is_wall(i, j))
				layer.set_value(i, j, temp[i][j]);
		}
	}
}

void normalize_solo_occupancy(MapLayer<float> &layer)
{
    /*
        Determine the maximum value in the given layer, and then divide the value
        for every cell in the layer by that amount.  This will keep the values in the
        range of [0, 1].  Negative values should be left unmodified.
    */

	float max_i = 0.0f;
	for (int i = 0; i < mapHeight; ++i)
	{
		for (int j = 0; j < mapWidth; ++j)
		{
			max_i = std::max(layer.get_value(i, j), max_i);
		}
	}
	if (max_i == 0.0f)
		return;
	for (int i = 0; i < mapHeight; ++i)
	{
		for (int j = 0; j < mapWidth; ++j)
		{
			if (layer.get_value(i, j) > 0.0f)
			{
				layer.set_value(i, j, layer.get_value(i, j) / max_i);				
			}
		}
	}
}

void normalize_dual_occupancy(MapLayer<float> &layer)
{
    /*
        Similar to the solo version, but you need to track greatest positive value AND 
        the least (furthest from 0) negative value.

        For every cell in the given layer, if the value is currently positive divide it by the
        greatest positive value, or if the value is negative divide it by -1.0 * the least negative value
        (so that it remains a negative number).  This will keep the values in the range of [-1, 1].
    */

	float max_i = 0.0f;
	float min_i = 0.0f;
	for (int i = 0; i < mapHeight; ++i)
	{
		for (int j = 0; j < mapWidth; ++j)
		{
			if (layer.get_value(i, j) > max_i)
			{
				max_i = layer.get_value(i, j);
			}
			else if (layer.get_value(i, j) < min_i)
			{
				min_i = layer.get_value(i, j);
			}
		}
	}
	if (max_i == 0.0f)
		max_i = 1.0f;
	if (min_i == 0.0f)
		min_i = 1.0f;
	else
		min_i *= -1.0f;
	for (int i = 0; i < mapHeight; ++i)
	{
		for (int j = 0; j < mapWidth; ++j)
		{
			if (layer.get_value(i, j) > 0.0f)
			{
				layer.set_value(i, j, layer.get_value(i, j) / max_i);
			}
			else if(layer.get_value(i, j) < 0.0f)
				layer.set_value(i, j, layer.get_value(i, j) / min_i);
		}
	}
}

void enemy_field_of_view(MapLayer<float> &layer, float fovAngle, float closeDistance, float occupancyValue, AStarAgent *enemy)
{
    /*
        First, clear out the old values in the map layer by setting any negative value to 0.
        Then, for every cell in the layer that is within the field of view cone, from the
        enemy agent, mark it with the occupancy value.  Take the dot product between the view
        vector and the vector from the agent to the cell, both normalized, and compare the
        cosines directly instead of taking the arccosine to avoid introducing floating-point
        inaccuracy (larger cosine means smaller angle).

        If the tile is close enough to the enemy (less than closeDistance),
        you only check if it's visible to enemy.  Make use of the is_clear_path
        helper function.  Otherwise, you must consider the direction the enemy is facing too.
        This creates a radius around the enemy that the player can be detected within, as well
        as a fov cone.
    */

	if (map != terrain->currentMap)
		update_visible_graph();

	for (int i = 0; i < mapHeight; ++i)
	{
		for (int j = 0; j < mapWidth; ++j)
		{
			if (layer.get_value(i, j) < 0.0f)
				layer.set_value(i, j, 0.0f);
		}
	}

	const auto & enemy_v = enemy->get_position();
	Vec3 enemy_fv_norm = enemy->get_forward_vector();
	Vec2 face(enemy_fv_norm.x, enemy_fv_norm.z);
	face.Normalize();
	GridPos enemy_pos = terrain->get_grid_position(enemy_v);
	static const float fov_cos = cosf(ANG_TO_RAD * fovAngle / 2);
	int vi = enemy_pos.row * mapHeight + enemy_pos.col;
	
	float dist_squre = closeDistance * closeDistance * dist_square_vec;
	for (int i = 0; i < mapHeight; ++i)
	{
		for (int j = 0; j < mapWidth; ++j)
		{
			if (!terrain->is_wall(i, j))
			{
				const auto & tile_v = terrain->get_world_position(i, j);
				if (Vec3::DistanceSquared(tile_v, enemy_v) <= dist_squre)
				{
					if(visible_graph[vi][i * mapHeight + j])
						layer.set_value(i, j, occupancyValue);
				}
				else
				{
					Vec3 & dir_v = terrain->get_world_position(i, j) - enemy_v;
					Vec2 tile_2(dir_v.x, dir_v.z);
					tile_2.Normalize();
					if (face.Dot(tile_2) >= fov_cos && visible_graph[vi][i * mapHeight + j])
					{
						layer.set_value(i, j, occupancyValue);
					}
				}
			}
		}
	}
}

bool enemy_find_player(MapLayer<float> &layer, AStarAgent *enemy, Agent *player)
{
    /*
        Check if the player's current tile has a negative value, ie in the fov cone
        or within a detection radius.
    */

    const auto &playerWorldPos = player->get_position();
    const auto playerGridPos = terrain->get_grid_position(playerWorldPos);

    // verify a valid position was returned
    if (terrain->is_valid_grid_position(playerGridPos) == true)
    {
        if (layer.get_value(playerGridPos) < 0.0f)
        {
            return true;
        }
    }

    // player isn't in the detection radius or fov cone, OR somehow off the map
    return false;
}

bool enemy_seek_player(MapLayer<float> &layer, AStarAgent *enemy)
{
    /*
        Attempt to find a cell with the highest nonzero value (normalization may
        not produce exactly 1.0 due to floating point error), and then set it as
        the new target, using enemy->path_to.

        If there are multiple cells with the same highest value, then pick the
        cell closest to the enemy.

        Return whether a target cell was found.
    */

	float max = 0.0f;
	GridPos enemy_pos = terrain->get_grid_position(enemy->get_position());
	GridPos target;
	int min_dist = -1;
	for (int i = 0; i < mapHeight; ++i)
	{
		for (int j = 0; j < mapWidth; ++j)
		{
			const float& value = layer.get_value(i, j);
			if (value > max)
			{
				max = value;
				target.row = i;
				target.col = j;
				min_dist = (i - enemy_pos.row) * (i - enemy_pos.row) + (j - enemy_pos.col) * (j - enemy_pos.col);
			}
			else if (value == max)
			{
				int dis = (i - enemy_pos.row) * (i - enemy_pos.row) + (j - enemy_pos.col) * (j - enemy_pos.col);
				if (dis < min_dist)
				{
					min_dist = dis;
					target.row = i;
					target.col = j;
				}
			}
		}
	}
	if (min_dist > 0)
	{
		enemy->path_to(terrain->get_world_position(target));
		return true;
	}
    return false;
}
