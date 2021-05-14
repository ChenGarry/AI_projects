#include <pch.h>
#include "L_TargetedWander.h"
#include "Agent/BehaviorAgent.h"

namespace {
    //float rad = 0.0174532925f;
    //float todegree = 57.2957795f;
    float maxRotation = 15.0 * 0.0174532925f;
    float towPiRad = 6.2831853072;
    float distance = 6.0f;
    float radius = 4.0f;
}

void L_TargetedWander::on_enter()
{
    // set animation, speed, etc
    auto &bd = agent->get_blackboard();
    targetPoint = bd.get_value<Vec3>("targetPoint");
    float r = RNG::range(0.0f, 6.2831853072f);
    targetPoint.x += sinf(r) * 20.0f;
    targetPoint.z += cosf(r) * 20.0f;

    timer = 0.0f;
	BehaviorNode::on_leaf_enter();
}

void L_TargetedWander::on_update(float dt)
{
    timer -= dt;

    if(timer <= 0.0f)
    {
		movingPoint = agent->get_position();
        Vec3 dir = targetPoint - movingPoint;
        dir.Normalize();
        dir *= distance;
        rotation += RNG::range(-maxRotation, maxRotation);
        if(rotation < 0)
            rotation += towPiRad;
        else if(rotation > towPiRad)
            rotation -= towPiRad;
		movingPoint += dir;
		movingPoint.x += sinf(rotation)*radius;
		movingPoint.z += cosf(rotation)*radius;
        timer = 0.05f;
    }

    const auto result = agent->move_toward_point(movingPoint, dt);
    const float distance = Vec3::Distance(movingPoint, targetPoint);
    if (distance <= 5.0f)
    {
        on_success();
    }

    display_leaf_text();
}
