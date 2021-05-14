#include <pch.h>
#include "L_MoveUntillBorder.h"
#include "Agent/BehaviorAgent.h"

void L_MoveUntillBorder::on_enter()
{
    // set animation, speed, etc

	BehaviorNode::on_leaf_enter();
    auto &bd = agent->get_blackboard();
	lane = bd.get_value<float>("lane");
}

void L_MoveUntillBorder::on_update(float dt)
{
    Vec3 currentPos = agent->get_position();

    switch(dir)
    {
        case 0: // right
            currentPos.z = 100 - lane;
			break;
        case 2: // left
            currentPos.z = lane;
			break;
        case 3: // down
            currentPos.x = lane;
			break;
        case 1: // up
            currentPos.x = 100 - lane;
			break;
        default:
            break;
    }

    const auto result = agent->move_toward_point(currentPos, dt);

    if (result == true)
    {
        ++dir;
        if(dir > 3)
            dir = 0;
    }
    on_success();

    display_leaf_text();
}
