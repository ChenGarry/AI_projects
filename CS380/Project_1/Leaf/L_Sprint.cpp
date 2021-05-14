#include <pch.h>
#include "L_Sprint.h"
#include "Agent/BehaviorAgent.h"

void L_Sprint::on_enter()
{
    // set animation, speed, etc

	BehaviorNode::on_leaf_enter();
    timer = 2.0f;
    agent->set_movement_speed(agent->get_movement_speed() * 2);
}

void L_Sprint::on_update(float dt)
{
    timer -= dt;
    if(timer <= 0)
    {
        agent->set_movement_speed(agent->get_movement_speed() / 2);
        on_success();
    }

    display_leaf_text();
}
