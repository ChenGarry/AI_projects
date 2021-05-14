#include <pch.h>
#include "L_CheckBorder.h"
#include "Agent/BehaviorAgent.h"

void L_CheckBorder::on_enter()
{
    // set animation, speed, etc
    result = false;

    // and our agent's position
    const auto &currPos = agent->get_position();
    if(currPos.x > 98.0f || currPos.x < 2.0f || currPos.z > 98.0f || currPos.z < 2.0f )
        on_success();
    else
        on_failure();    
}

void L_CheckBorder::on_update(float dt)
{
    
    if (result == true)
    {
        on_success();
    }
    else
        on_failure();

    display_leaf_text();
}
