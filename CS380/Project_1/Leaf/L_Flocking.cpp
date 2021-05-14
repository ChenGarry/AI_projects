#include <pch.h>
#include "L_Flocking.h"
#include "Agent/BehaviorAgent.h"

namespace {
    //float rad = 0.0174532925f;
    //float todegree = 57.2957795f;
    float maxRotation = 15.0 * 0.0174532925f;
    float towPiRad = 6.2831853072;
    float wander_distance = 5.0f;
    float wander_radius = 0.5f;
    float separation_dist = 10.0f;
    float alignment_dist = 15.0f;
    float cohesion_dist = 15.0f;
    float maxspeed = 25.0f;
}

void L_Flocking::on_enter()
{
    // set animation, speed, etc

    // get a list of all current agents
	BehaviorNode::on_leaf_enter();
}

void L_Flocking::on_update(float dt)
{
    const auto targetPoint = agent->get_blackboard().get_value<Vec3>("targetPoint");
	const auto &allAgents = agents->get_all_agents();
    const auto &currPos = agent->get_position();
    Vec3 currV = agent->get_forward_vector();
    const float currSpeed = agent->get_movement_speed();
    currV.Normalize();
    currV *= currSpeed;
    Vec3 wander_dir = targetPoint - currPos;
    wander_dir.Normalize();
    wander_dir *= wander_distance;
    rotation += RNG::range(-maxRotation, maxRotation);
    if(rotation < 0)
        rotation += towPiRad;
    else if(rotation > towPiRad)
        rotation -= towPiRad;
    wander_dir += currPos;
    wander_dir.x += sinf(rotation)*wander_radius;
    wander_dir.z += cosf(rotation)*wander_radius;
    wander_dir.Normalize();
    wander_dir *= maxspeed;
    wander_dir -= currV;

    int seperate_count = 0;
    int alignment_count = 0;
    int cohesion_count = 0;
    Vec3 seperate_steer(0.0f, 0.0f, 0.0f);
    Vec3 alignment_steer(0.0f, 0.0f, 0.0f);
    Vec3 seek(0.0f, 0.0f, 0.0f);

    for (const auto & a : allAgents)
    {
        // make sure it's not our agent
        if (a != agent && a->get_type() == "Boid" )
        {
            const auto &agentPos = a->get_position();
            Vec3 agentV = a->get_forward_vector();
            const float agentSpeed = a->get_movement_speed();
            const float distance = Vec3::Distance(currPos, agentPos);
            agentV.Normalize();
            agentV *= agentSpeed;

            if (distance > 0 && distance <= separation_dist)
            {
                Vec3 diff = currPos - agentPos;
                diff.Normalize();
                diff /= distance;
                seperate_steer += diff;
                ++seperate_count;
            }
            if(distance > 0 && distance <= alignment_dist)
            {
                seek += agentPos;
                ++cohesion_count;
                alignment_steer += agentV;
                ++alignment_count;
            }
        }
    }
    if(seperate_count > 0)
    {
        seperate_steer /= (float)seperate_count;
        seperate_steer.Normalize();
        seperate_steer *= maxspeed;
        seperate_steer -= currV;
    }
    if(cohesion_count > 0)
    {
        seek /= (float)cohesion_count;
        seek -= currPos;
        seek.Normalize();
        seek *= maxspeed;
        seek -= currV;
    }
    if(alignment_count > 0)
    {
        alignment_steer /= (float)alignment_count;
        alignment_steer.Normalize();
        alignment_steer *= maxspeed;
        alignment_steer -= currV;
    }

    float s = 0.42857142857f * 0.7;
    float se = 0.28571428571f * 0.7;
    float al = 0.28571428571f * 0.7;
    Vec3 acc = seperate_steer * s + seek*se + alignment_steer*al + wander_dir * 0.1f;
    currV += acc * dt;
    float speed = currV.Length();
    if(speed > maxspeed)
        speed = maxspeed;
    //speed *= RNG::range(0.8f, 1.1f);
    agent->set_movement_speed(speed);
    agent->move_toward_point(currPos + currV, dt);
    on_success();

    //display_leaf_text();
}
