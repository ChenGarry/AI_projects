#include <pch.h>
#include "L_WaveHand.h"
#include "Agent/BehaviorAgent.h"

namespace {
    float rad = 0.0174532925f;
    float todegree = 57.2957795f;
    float maxroll = 45.0 * 0.0174532925f;
}
void L_WaveHand::on_enter()
{
    // set animation, speed, etc
	BehaviorNode::on_leaf_enter();
    count = 3;
}

void L_WaveHand::on_update(float dt)
{
    float old_rotation = agent->get_roll();
    float new_rotation = old_rotation + (dir * waveSpeed * dt) * rad;
    agent->set_roll(new_rotation);
    
    if(new_rotation * old_rotation < 0)
        --count;

    if (count == 0)
    {
        on_success();
    }
    else if(new_rotation * dir >= maxroll)
        dir *= -1;

    display_leaf_text();
}

void L_WaveHand::on_exit()
{    
    agent->set_roll(0);
    set_status(NodeStatus::SUSPENDED);
}
