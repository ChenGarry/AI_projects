#pragma once
#include "Misc/PathfindingDetails.hpp"

namespace {
#define NOT_VISIST 0
#define ON_OPEN 1
#define ON_CLOSE 2

uint16_t PosToBits(int row, int col);
void BitsToPos(uint16_t &bits, int &row, int &col);

struct Node
{
	float cost;    		// total cost = g+h
	float given;   		// given cost
	uint16_t bit_flags; // onlist flag 2 bit, parent x 6 bits, y 6 bits
	void setParent(uint16_t row, uint16_t col);
	void setListFlag(uint16_t flag);
	void getParent(int &row, int& col);
	uint16_t getListFlag();
};
	
class BucketSort
{
	friend class AStarPather;
	struct Bucket
	{
		int end;
		uint16_t list[90];
		Bucket() : end(0), list() {};
	};
	const static int end = 2500;
	Bucket buckets[end];
	Node (*_map)[40][40];
	int min_bucket;
	int max_count;
	int max_i;
	void push(float key, uint16_t val);
	uint16_t pop_min();
	void update(float old_key, float new_key, uint16_t val);
	bool empty();
	void initialize();
	void updateMin();
	BucketSort() : buckets(), min_bucket(end) {}
};
}




class AStarPather
{
	friend class BucketSort;
public:
    /* 
        The class should be default constructible, so you may need to define a constructor.
        If needed, you can modify the framework where the class is constructed in the
        initialize functions of ProjectTwo and ProjectThree.
    */
    /* ************************************************** */
    // DO NOT MODIFY THESE SIGNATURES
    bool initialize();
    void shutdown();
    PathResult compute_path(PathRequest &request);
    /* ************************************************** */

    /*
        You should create whatever functions, variables, or classes you need.
        It doesn't all need to be in this header and cpp, structure it whatever way
        makes sense to you.
    */
private:
	int dir[8][2] = { {0,-1}, {-1,-1}, {-1,0}, {-1,1}, {0,1}, {1,1}, {1,0}, {1,-1} };
    Node _map[40][40];
    BucketSort _openList;
	GridPos start;
	GridPos goal;
	int mapHeight;
	int mapWidth;
	float (AStarPather::*HeuristicFn)(int, int);
	void iniMap();

	PathResult AStar(PathRequest &request);
	void AStarUpdateChild();
	float HeuristicOctile(int xDiff, int yDiff);
	float HeuristicChebyshev(int xDiff, int yDiff);
	float HeuristicManhattan(int xDiff, int yDiff);
	float HeuristicEuclidean(int xDiff, int yDiff);

	void DebugColoring();
	void FillPath(PathRequest &request);
	void RubberBanding(PathRequest &request);
	bool HasWall(int a_row, int a_col, int b_row, int b_col);
	void Smoothing(PathRequest &request);
};