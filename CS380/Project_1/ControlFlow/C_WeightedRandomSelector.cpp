#include <pch.h>
#include "C_WeightedRandomSelector.h"

namespace
{
}

C_WeightedRandomSelector::C_WeightedRandomSelector() : randomIndex(-1), weights{95,100}
{}

void C_WeightedRandomSelector::on_enter()
{
    BehaviorNode::on_enter();

    choose_random_node();
}

void C_WeightedRandomSelector::on_update(float dt)
{
    // if any child succeeds, the node succeeds
    // if all children fail, the node fails
    BehaviorNode *currentNode = children[randomIndex];
    currentNode->tick(dt);

    if (currentNode->succeeded() == true)
    {
        on_success();
    }
    else if (currentNode->failed() == true)
    {
        // first check if every child has failed
        if (check_for_all_failed() == true)
        {
            // the node has failed
            on_failure();
        }
        else
        {
            // choose the next node to try
            choose_random_node();
        }
    }
}

void C_WeightedRandomSelector::choose_random_node()
{
    // choose a random node that hasn't succeeded or failed
    while (true)
    {
        randomIndex = RNG::range(static_cast<size_t>(0), weights[1]-1);
        for(size_t i = 0; i < 2; ++i)
        {
            if(randomIndex < weights[i])
            {
                randomIndex = i;
                break;
            }
        }

        BehaviorNode *node = children[randomIndex];
        if (node->is_ready() == true)
        {
            break;
        }
    }
}

bool C_WeightedRandomSelector::check_for_all_failed() const
{
    bool result = true;

    for (const auto & child : children)
    {
        result &= child->failed();
    }

    return result;
}
