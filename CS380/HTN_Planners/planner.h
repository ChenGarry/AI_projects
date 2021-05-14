#ifndef PLANNER
#define PLANNER
#include <utility>   // std::pair
#include <vector>
#include "task.h"

enum EWorldState{
  EWS_1,
  EWS_count
};

extern std::vector<char> currentWordState;

void ChangeWord();

class HTNPlanner {
    private:
    public:
        HTNPlanner( int s );
        void FindPlan(std::vector<int> &plan, Tasks RootTask);
};

#endif
