#include <pch.h>
#include "L_Die.h"
#include "Agent/BehaviorAgent.h"

namespace {
    //float rad = 0.0174532925f;
    //float todegree = 57.2957795f;
    float maxrad = 30.0 * 0.0174532925f;
    float rate = 0.1f;
}
void L_Die::on_enter()
{
    // set animation, speed, etc

	BehaviorNode::on_leaf_enter();
    timer = 1.5f;
    //rateTimer = rate;
}

void L_Die::on_update(float dt)
{
    //if(rateTimer <= 0)
    //{
        agent->set_roll(RNG::range(-maxrad, maxrad));
        agent->set_pitch(RNG::range(-maxrad, maxrad));
        //rateTimer = rate;
    //}
    timer -= dt;
    //rate -= dt;
    if (timer <= 0)
    {
		//agent->set_dead();
        on_success();
    }
    display_leaf_text();
}

void L_Die::on_exit()
{
    // set animation, speed, etc
    agent->set_roll(0);
    agent->set_pitch(0);
	set_status(NodeStatus::SUSPENDED);
}
