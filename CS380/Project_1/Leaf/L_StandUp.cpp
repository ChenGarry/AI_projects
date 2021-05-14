#include <pch.h>
#include "L_StandUp.h"
#include "Agent/BehaviorAgent.h"

void L_StandUp::on_enter()
{
    // set animation, speed, etc

	BehaviorNode::on_leaf_enter();
    o_scales = agent->get_scaling();
    standing = true;
}

void L_StandUp::on_update(float dt)
{
    Vec3 new_scales = o_scales;
    new_scales.y *= scale;
    agent->set_scaling(new_scales);

    if (standing)
    {
        if(scale >= 2.0f)
            standing = false;
        scale += dt * 1.0f;
    }
    else
    {
        if(scale <= 1.0f)
            on_success();
        scale -= dt * 1.0f;
    }

    display_leaf_text();
}

void L_StandUp::on_exit()
{
	agent->set_scaling(o_scales);
    set_status(NodeStatus::SUSPENDED);
}
