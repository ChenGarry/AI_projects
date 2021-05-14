#include <pch.h>
#include "L_ChangeColor.h"
#include "Agent/BehaviorAgent.h"

void L_ChangeColor::on_enter()
{
    // set animation, speed, etc

	BehaviorNode::on_leaf_enter();
    
}

void L_ChangeColor::on_update(float dt)
{
    colorIndex = agent->get_blackboard().get_value<int>("colorIndex");
    if(colorIndex == 0)
    {
		//agent->set_color(Vec3(2.0f, 0.0f, 0.0f));
        Color = agent->get_blackboard().get_value<Vec3>("Color0");
		agent->set_color(Color);
        agent->get_blackboard().set_value<int>("colorIndex", 1);
    }
    else if(colorIndex == 1)
    {
		//agent->set_color(Vec3(2.0f, 2.0f, 0.0f)); 
        Color = agent->get_blackboard().get_value<Vec3>("Color1");
		agent->set_color(Color);
        agent->get_blackboard().set_value<int>("colorIndex", 0);
    }
    on_success();

    display_leaf_text();
}
