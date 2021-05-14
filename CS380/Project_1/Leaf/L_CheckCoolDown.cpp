#include <pch.h>
#include "L_CheckCoolDown.h"
#include "Agent/BehaviorAgent.h"

void L_CheckCoolDown::on_enter()
{
    // set animation, speed, etc

	BehaviorNode::on_leaf_enter();
}

void L_CheckCoolDown::on_update(float dt)
{
    timer -= dt;
    if(timer > 0)
    {
        on_failure();
    }
    else
    {
		timer = agent->get_blackboard().get_value<float>("cooldown");
        on_success();
    }

    display_leaf_text();
}
