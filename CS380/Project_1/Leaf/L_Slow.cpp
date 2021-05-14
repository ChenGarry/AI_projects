#include <pch.h>
#include "L_Slow.h"
#include "Agent/BehaviorAgent.h"

void L_Slow::on_enter()
{
    // set animation, speed, etc

	BehaviorNode::on_leaf_enter();
    timer = 2.5f;
    agent->set_movement_speed(agent->get_movement_speed() / 2);
}

void L_Slow::on_update(float dt)
{
    timer -= dt;
    if(timer <= 0)
    {
        agent->set_movement_speed(agent->get_movement_speed() * 2);
        on_success();
    }

    display_leaf_text();
}
