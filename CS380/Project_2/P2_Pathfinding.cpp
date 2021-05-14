#include <pch.h>
#include "Projects/ProjectTwo.h"
#include "P2_Pathfinding.h"
#include <cstdint>

#pragma region Extra Credit
bool ProjectTwo::implemented_floyd_warshall()
{
    return false;
}

bool ProjectTwo::implemented_goal_bounding()
{
    return false;
}

bool ProjectTwo::implemented_jps_plus()
{
    return false;
}
#pragma endregion

bool AStarPather::initialize()
{
    // handle any one-time setup requirements you have

    /*
        If you want to do any map-preprocessing, you'll need to listen
        for the map change message.  It'll look something like this:

        Callback cb = std::bind(&AStarPather::your_function_name, this);
        Messenger::listen_for_message(Messages::MAP_CHANGE, cb);

        There are other alternatives to using std::bind, so feel free to mix it up.
        Callback is just a typedef for std::function<void(void)>, so any std::invoke'able
        object that std::function can wrap will suffice.
    */
	_openList._map = &_map;
    return true; // return false if any errors actually occur, to stop engine initialization
}

void AStarPather::shutdown()
{
    /*
        Free any dynamically allocated memory or any other general house-
        keeping you need to do during shutdown.
    */
}

PathResult AStarPather::compute_path(PathRequest &request)
{
    /*
        This is where you handle pathing requests, each request has several fields:

        start/goal - start and goal world positions
        path - where you will build the path upon completion, path should be
            start to goal, not goal to start
        heuristic - which heuristic calculation to use
        weight - the heuristic weight to be applied
        newRequest - whether this is the first request for this path, should generally
            be true, unless single step is on

        smoothing - whether to apply smoothing to the path
        rubberBanding - whether to apply rubber banding
        singleStep - whether to perform only a single A* step
        debugColoring - whether to color the grid based on the A* state:
            closed list nodes - yellow
            open list nodes - blue

            use terrain->set_color(row, col, Colors::YourColor);
            also it can be helpful to temporarily use other colors for specific states
            when you are testing your algorithms

        method - which algorithm to use: A*, Floyd-Warshall, JPS+, or goal bounding,
            will be A* generally, unless you implement extra credit features

        The return values are:
            PROCESSING - a path hasn't been found yet, should only be returned in
                single step mode until a path is found
            COMPLETE - a path to the goal was found and has been built in request.path
            IMPOSSIBLE - a path from start to goal does not exist, do not add start position to path
    */

	PathResult result;
	if (request.newRequest)
	{
		start = terrain->get_grid_position(request.start);
		goal = terrain->get_grid_position(request.goal);
		if(start == goal)
			return PathResult::COMPLETE;

		mapHeight = terrain->get_map_height();
		mapWidth = terrain->get_map_width();
		iniMap();
		_openList.initialize();
		_openList.push(0, PosToBits(start.row, start.col));
	}

	switch (request.settings.heuristic)
	{
		case Heuristic::OCTILE:
			HeuristicFn = &AStarPather::HeuristicOctile;
			break;
		case Heuristic::CHEBYSHEV:
			HeuristicFn = &AStarPather::HeuristicChebyshev;
			break;
		case Heuristic::MANHATTAN:
			HeuristicFn = &AStarPather::HeuristicManhattan;
			break;
		case Heuristic::EUCLIDEAN:
			HeuristicFn = &AStarPather::HeuristicEuclidean;
			break;
		default:
			return PathResult::IMPOSSIBLE;
	}

	switch (request.settings.method)
	{
		case Method::ASTAR:
			result = AStar(request);
			break;
		case Method::FLOYD_WARSHALL:
			//break;
		case Method::GOAL_BOUNDING:
			//break;
		default:
			return PathResult::IMPOSSIBLE;
	}

	
	if (request.settings.debugColoring)
		DebugColoring();

	if (result == PathResult::COMPLETE)
	{
		if (request.settings.rubberBanding)
			RubberBanding(request);
		else
			FillPath(request);
		if (request.settings.smoothing)
			Smoothing(request);
		return PathResult::COMPLETE;
	}
	else if (result == PathResult::IMPOSSIBLE)
	{
		return PathResult::IMPOSSIBLE;
	}

	return PathResult::PROCESSING;
}

PathResult AStarPather::AStar(PathRequest &request)
{
	int p_row;
	int p_col;
	int n_row;
	int n_col;
	float w = 1.0000001f * request.settings.weight;
	while(!_openList.empty())
	{
		 uint16_t parent = _openList.pop_min();
		 BitsToPos(parent, p_row, p_col);
		 _map[p_row][p_col].setListFlag(ON_CLOSE);
		 if(p_row == goal.row && p_col == goal.col)
		 {
			 return PathResult::COMPLETE;
		 }
		 bool valid[9];
		 for(int i = 0; i < 7; i += 2)
		 {
			 n_row = p_row + dir[i][0];
			 n_col = p_col + dir[i][1];
			 if (terrain->is_valid_grid_position(n_row, n_col) && !terrain->is_wall(n_row, n_col))
			 {
				 float g = _map[p_row][p_col].given + 1.0f;
				 float f = w * (this->*HeuristicFn)(std::abs(n_row - goal.row), std::abs(n_col - goal.col)) + g;
				 uint16_t flag = _map[n_row][n_col].getListFlag();
				 if (flag == NOT_VISIST)
				 {
					 _map[n_row][n_col].setListFlag(ON_OPEN);
					 _map[n_row][n_col].setParent(p_row, p_col);
					 _map[n_row][n_col].given = g;
					 _map[n_row][n_col].cost = f;
					 _openList.push(f, PosToBits(n_row, n_col));
				 }
				 else if (flag == ON_OPEN && f < _map[n_row][n_col].cost)
				 {
					 _map[n_row][n_col].setParent(p_row, p_col);
					 _openList.update(_map[n_row][n_col].cost, f, PosToBits(n_row, n_col));
					 _map[n_row][n_col].given = g;
					 _map[n_row][n_col].cost = f;
				 }
				 else if (flag == ON_CLOSE && f < _map[n_row][n_col].cost)
				 {
					 _map[n_row][n_col].setListFlag(ON_OPEN);
					 _map[n_row][n_col].setParent(p_row, p_col);
					 _map[n_row][n_col].given = g;
					 _map[n_row][n_col].cost = f;
					 _openList.push(f, PosToBits(n_row, n_col));
				 }
				 valid[i] = true;
			 }
			 else
				 valid[i] = false;
		 }
		 valid[8] = valid[0];

		 for(int i = 1; i < 8; i += 2)
		 {
			 n_row = p_row + dir[i][0];
			 n_col = p_col + dir[i][1];
			 if(valid[i-1] && valid[i+1] && !terrain->is_wall(n_row, n_col))
			 {
				 float g = _map[p_row][p_col].given + sqrt(2.0f);
				 float f = w * (this->*HeuristicFn)(std::abs(n_row - goal.row), std::abs(n_col - goal.col)) + g;
				 uint16_t flag = _map[n_row][n_col].getListFlag();
				 if (flag == NOT_VISIST)
				 {
					 _map[n_row][n_col].setListFlag(ON_OPEN);
					 _map[n_row][n_col].setParent(p_row, p_col);
					 _map[n_row][n_col].given = g;
					 _map[n_row][n_col].cost = f;
					 _openList.push(f, PosToBits(n_row, n_col));
				 }
				 else if (flag == ON_OPEN && f < _map[n_row][n_col].cost)
				 {
					 _map[n_row][n_col].setParent(p_row, p_col);
					 _openList.update(_map[n_row][n_col].cost, f, PosToBits(n_row, n_col));
					 _map[n_row][n_col].given = g;
					 _map[n_row][n_col].cost = f;
				 }
				 else if (flag == ON_CLOSE && f < _map[n_row][n_col].cost)
				 {
					 _map[n_row][n_col].setListFlag(ON_OPEN);
					 _map[n_row][n_col].setParent(p_row, p_col);
					 _map[n_row][n_col].given = g;
					 _map[n_row][n_col].cost = f;
					 _openList.push(f, PosToBits(n_row, n_col));
				 }
			 }
		 }
		 if(request.settings.singleStep)
		 {
			 return PathResult::PROCESSING;
		 }
	}
	return PathResult::IMPOSSIBLE;
}

void AStarPather::iniMap()
{
	memset(_map, 0, 1600 * sizeof(Node));
}

float AStarPather::HeuristicOctile(int smallerDiff, int lagerDiff)
{
	if (smallerDiff > lagerDiff)
		std::swap(smallerDiff, lagerDiff);
	return smallerDiff * sqrt(2.0f) + lagerDiff - smallerDiff;
}

float AStarPather::HeuristicChebyshev(int xDiff, int yDiff)
{
	return static_cast<float>(std::max(xDiff, yDiff));
}
float AStarPather::HeuristicManhattan(int xDiff, int yDiff)
{
	return static_cast<float>(xDiff + yDiff);
}
float AStarPather::HeuristicEuclidean(int xDiff, int yDiff)
{
	return static_cast<float>(sqrt(xDiff * xDiff + yDiff * yDiff));
}

void AStarPather::DebugColoring()
{
	for (int i = 0; i < mapHeight; ++i)
	{
		for (int j = 0; j < mapWidth; ++j)
		{
			uint16_t state = _map[i][j].getListFlag();
			if(state == ON_CLOSE)
				terrain->set_color(i, j, Colors::Yellow);
			else if (state == ON_OPEN)
				terrain->set_color(i, j, Colors::Blue);
		}
	}
}


void AStarPather::FillPath(PathRequest &request)
{
	int row = goal.row;
	int col = goal.col;
	do
	{
		request.path.push_front(terrain->get_world_position(row, col));
		_map[row][col].getParent(row, col);
	} while (row != start.row || col != start.col);
	request.path.push_front(terrain->get_world_position(row, col));
}

void AStarPather::RubberBanding(PathRequest &request)
{
	int row = goal.row;
	int col = goal.col;
	int mid_r;
	int mid_c;
	int end_r;
	int end_c;
	do
	{
		request.path.push_front(terrain->get_world_position(row, col));
		_map[row][col].getParent(mid_r, mid_c);
		while ((mid_r != start.row || mid_c != start.col))
		{
			_map[mid_r][mid_c].getParent(end_r, end_c);
			if (HasWall(end_r, end_c, row, col))
				break;
			mid_r = end_r;
			mid_c = end_c;
		}
		row = mid_r;
		col = mid_c;
	} while (row != start.row || col != start.col);
	request.path.push_front(terrain->get_world_position(row, col));
}

bool AStarPather::HasWall(int a_row, int a_col, int b_row, int b_col)
{
	if (a_row > b_row)
		std::swap(a_row, b_row);
	if (a_col > b_col)
		std::swap(a_col, b_col);
	for (int row = a_row; row <= b_row; ++row)
	{
		for (int col = a_col; col <= b_col; ++col)
		{
			if (terrain->is_wall(row, col))
			{
				return true;
			}
		}
	}
	return false;
}

void AStarPather::Smoothing(PathRequest &request)
{
	float max_gap = 1.5f * Vec3::Distance(terrain->get_world_position(1, 0), terrain->get_world_position(0, 0));
	float dist;
	auto it = request.path.begin();
	auto next_it = it;
	++next_it;

	while (next_it != request.path.end())
	{
		dist = Vec3::Distance(*it, *next_it);
		if (dist > max_gap)
		{
			request.path.insert(next_it, (*it + *next_it) / 2);
		}
		else
			it++;
		next_it = it;
		++next_it;
	}

	if (request.path.size() < 3) 
		return;
	it = request.path.begin();
	auto p1 = it;
	auto p2 = it;
	auto p3 = ++it;
	auto p4 = ++it;
	while (p4 != request.path.end())
	{
		for (int i = 1; i < 4; ++i)
		{
			request.path.insert(p3, Vec3::CatmullRom(*p1, *p2, *p3, *p4, 0.25f * i));
		}
		p1 = p2;
		p2 = p3;
		p3 = p4;
		++p4;
	}
	for (int i = 1; i < 4; ++i)
	{
		request.path.insert(p3, Vec3::CatmullRom(*p1, *p2, *p3, *p3, 0.25f * i));
	}
}

namespace
{
#define X_MASK 4032
#define Y_MASK 63
#define FLAG_MASK 12288
#define INTERVAL 15

uint16_t PosToBits(int row, int col)
{
	return static_cast<uint16_t>((row << 6) + col);
}

void BitsToPos(uint16_t &bits, int &row, int &col)
{
	row = static_cast<int>((bits & X_MASK) >> 6);
	col = static_cast<int>(bits & Y_MASK);
}

void Node::setParent(uint16_t row, uint16_t col)
{
	bit_flags &= FLAG_MASK;
	bit_flags |= row << 6;
	bit_flags |= col;
}
void Node::setListFlag(uint16_t flag)
{
	bit_flags &= ~FLAG_MASK;
	bit_flags |= flag << 12;
}
void Node::getParent(int &row, int& col)
{
	row = static_cast<int>((bit_flags & X_MASK) >> 6);
	col = static_cast<int>(bit_flags & Y_MASK);
}
uint16_t Node::getListFlag()
{
	return (bit_flags & FLAG_MASK) >> 12;
}

void BucketSort::push(float key, uint16_t val)
{
	int i = static_cast<int>(key * INTERVAL);
	int &b_end = buckets[i].end;
	buckets[i].list[b_end] = val;
	++b_end;
	if (i < min_bucket)
		min_bucket = i;
	max_count = std::max(max_count, b_end);
	max_i = std::max(max_i, i);
}

uint16_t BucketSort::pop_min()
{
	int &b_end = buckets[min_bucket].end;
	int row;
	int col;
	int min_i = 0;
	BitsToPos(buckets[min_bucket].list[0], row, col);
	float min_cost = (*_map)[row][col].cost;
	uint16_t coordinates = buckets[min_bucket].list[0];

	for (int i = 1; i < b_end; ++i)
	{
		BitsToPos(buckets[min_bucket].list[i], row, col);
		if ((*_map)[row][col].cost < min_cost)
		{
			min_i = i;
			min_cost = (*_map)[row][col].cost;
			coordinates = buckets[min_bucket].list[i];
		}
	}
	--b_end;
	std::swap(buckets[min_bucket].list[min_i], buckets[min_bucket].list[b_end]);
	if (b_end == 0)
		updateMin();
	return coordinates;
}

void BucketSort::update(float old_key, float new_key, uint16_t val)
{
	int i = static_cast<int>(old_key * INTERVAL);
	int new_i = static_cast<int>(new_key * INTERVAL);
	if (i == new_i)
		return;

	int &b_end = buckets[i].end;
	for (int j = 0; j < b_end; ++j)
	{
		if (buckets[i].list[j] == val)
		{
			--b_end;
			std::swap(buckets[i].list[j], buckets[i].list[b_end]);
			break;
		}
	}
	push(new_key, val);
	if (b_end == 0)
		updateMin();
}

bool BucketSort::empty()
{
	return (min_bucket == BucketSort::end);
}


void BucketSort::updateMin()
{
	for (int i = min_bucket; i < BucketSort::end; ++i)
	{
		if (buckets[i].end != 0)
		{
			min_bucket = i;
			return;
		}
	}
	min_bucket = BucketSort::end;
}

void BucketSort::initialize()
{
    memset(buckets, 0, BucketSort::end * sizeof(Bucket));
	min_bucket = 0;
	max_count = 0;
	max_i = 0;
}
}