#include <pch.h>
#include "L_Wander.h"
#include "Agent/BehaviorAgent.h"

namespace {
    //float rad = 0.0174532925f;
    //float todegree = 57.2957795f;
    float maxRotation = 15.0 * 0.0174532925f;
    float towPiRad = 6.2831853072;
    float distance = 10.0f;
    float radius = 4.0f;
}
void L_Wander::on_enter()
{
    // set animation, speed, etc

	BehaviorNode::on_leaf_enter();    
    targetPoint = agent->get_position();
    Vec3 dir = agent->get_forward_vector();
    dir.Normalize();
    dir *= distance;
    rotation += RNG::range(-maxRotation, maxRotation);
    if(rotation < 0)
        rotation += towPiRad;
    else if(rotation > towPiRad)
        rotation -= towPiRad;
    targetPoint += dir;
    targetPoint.x += sinf(rotation) * radius;
    targetPoint.z += cosf(rotation) * radius;
    // if(targetPoint.x < 0 || targetPoint.x > 100 || targetPoint.z < 0 || targetPoint.z > 100)
    // {
    //     targetPoint = Vec3(50.0f, 0.0f, 50.0f);
    //     //targetPoint -= agent->get_position();
    //     timer = 0.5f;
    // }
    timer = 0.00f;
}

void L_Wander::on_update(float dt)
{
    agent->move_toward_point(targetPoint, dt);
    //timer -= dt;
    if(timer <= 0)
        on_success();

    display_leaf_text();
}
