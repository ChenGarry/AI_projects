#include "planner.h"
#include <queue>
#include <stack>
std::vector<char> currentWordState(EWS_count, '0');

void ChangeWord() {
  currentWordState.push_back(currentWordState.back()+1);
}

void HTNPlanner::FindPlan(std::vector<int> &plan, Tasks RootTask)
{
    auto WorkingWS = currentWordState;
    std::stack<Tasks> TasksToProcess;
    TasksToProcess.push(RootTask);
    plan[0] = 0;

    // while(!TasksToProcess.empty())
    // {
    //     unsigned CurrentTask = TasksToProcess.top();
    //     if CurrentTask.Type == CompoundTask
    //     {
    //         SatisfiedMethod = CurrentTask.FindSatisfiedMethod(WorkingWS)
    //         if SatisfiedMethod != null
    //         {
    //             RecordDecompositionOfTask(CurrentTask, FinalPlan, DecompHistory)
    //             TasksToProcess.InsertTop(SatisfiedMethod.SubTasks)
    //         }
    //         else
    //         {
    //             RestoreToLastDecomposedTask()
    //         }
    //     }
    //     else//Primitive Task
    //     {
    //         if PrimitiveConditionMet(CurrentTask)
    //         {
    //             WorkingWS.ApplyEffects(CurrentTask.Effects)
    //             FinalPlan.PushBack(CurrentTask)
    //         }
    //         else
    //         {
    //             RestoreToLastDecomposedTask()
    //         }
    //     }
    // }
}