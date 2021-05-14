#include <pch.h>
#include "L_JitterAroundCenter.h"
#include "Agent/BehaviorAgent.h"

void L_JitterAroundCenter::on_enter()
{
    // set animation, speed, etc

	BehaviorNode::on_leaf_enter();
    auto &bd = agent->get_blackboard();
	facing = agent->get_yaw();
    targetPoint = bd.get_value<Vec3>("center");
    targetPoint.x += RNG::range(0.0f, 1.0f) * radius;
    targetPoint.z += RNG::range(0.0f, 1.0f) * radius;
}

void L_JitterAroundCenter::on_update(float dt)
{
    const auto result = agent->move_toward_point(targetPoint, dt);

    if (result == true)
    {
		agent->set_yaw(facing);
        on_success();
    }

    display_leaf_text();
}
