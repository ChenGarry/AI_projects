#include <pch.h>
#include "L_CheckTargetInZone.h"
#include "Agent/BehaviorAgent.h"

void L_CheckTargetInZone::on_enter()
{
    // set animation, speed, etc
    result = false;

    // get a list of all current agents
    const auto &allAgents = agents->get_all_agents();
    // and our agent's position
    //const auto &currPos = agent->get_position();
	const auto &currPos = agent->get_blackboard().get_value<Vec3>("center");
    //const float curRadius = (agent->get_scaling()).Length();

    for (const auto & a : allAgents)
    {
        // make sure it's not our agent
        if (a != agent && a->get_type() == "Runner1")
        {
            const auto &agentPos = a->get_position();
            const float zd = agentPos.z - currPos.z;

            if (zd <= 2.0f && zd >= -2.0f && agentPos.x > 95.0f)
            {
                on_success();
                result = true;
                return;
            }
        }
    }

    on_failure();
}

void L_CheckTargetInZone::on_update(float dt)
{
    
    if (result == true)
    {
        on_success();
    }
    else
        on_failure();

    display_leaf_text();
}
