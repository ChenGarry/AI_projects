#include <pch.h>
#include "L_CheckCollision.h"
#include "Agent/BehaviorAgent.h"

void L_CheckCollision::on_enter()
{
    // set animation, speed, etc
    result = false;

    // get a list of all current agents
    const auto &allAgents = agents->get_all_agents();

    // and our agent's position
    const auto &currPos = agent->get_position();
    //const float curRadius = (agent->get_scaling()).Length();

    for (const auto & a : allAgents)
    {
        // make sure it's not our agent
        if (a != agent)
        {
            const auto &agentPos = a->get_position();
            const float distance = Vec3::Distance(currPos, agentPos);
            //const float agentRadius = (a->get_scaling()).Length();

            if (distance <= 2.0f)
            {
                std::cout << "collide \n";
                on_success();
                result = true;
                return;
            }
        }
    }

    on_failure();
}

void L_CheckCollision::on_update(float dt)
{
    
    if (result == true)
    {
        on_success();
    }
    else
        on_failure();

    display_leaf_text();
}
