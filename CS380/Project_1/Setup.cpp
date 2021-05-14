#include <pch.h>
#include "Projects/ProjectOne.h"
#include "Agent/CameraAgent.h"

//x is lengh, z is width, y is height
void ProjectOne::setup()
{
#define rad 0.0174532925
    // Create your inital agents

	//ea->set_pitch(90.0f * rad);
	//ea->set_scaling(Vec3(20.0f, 1.0f, 20.0f));
	auto ea = agents->create_behavior_agent("Audience1", BehaviorTreeTypes::Audience);
	ea->set_position(Vec3(104.0f, 0.0f, 10.0f));
	ea->set_color(Vec3(1.0f, 1.0f, 1.0f));
	ea->set_scaling(Vec3(2.0f, 2.0f, 2.0f));
	ea->get_blackboard().set_value<Vec3>("center", ea->get_position());
	ea->get_blackboard().set_value<float>("cooldown", 0.5f);
	ea->set_yaw(-90.0f * 0.0174532925);

	ea = agents->create_behavior_agent("Audience2", BehaviorTreeTypes::Audience);
	ea->set_position(Vec3(104.0f, 0.0f, 30.0f));
	ea->set_color(Vec3(1.0f, 1.0f, 1.0f));
	ea->set_scaling(Vec3(2.0f, 2.0f, 2.0f));
	ea->get_blackboard().set_value<Vec3>("center", ea->get_position());
	ea->get_blackboard().set_value<float>("cooldown", 0.5f);
	ea->set_yaw(-90.0f * 0.0174532925);

	ea = agents->create_behavior_agent("Audience3", BehaviorTreeTypes::Audience);
	ea->set_position(Vec3(104.0f, 0.0f, 50.0f));
	ea->set_color(Vec3(1.0f, 1.0f, 1.0f));
	ea->set_scaling(Vec3(2.0f, 2.0f, 2.0f));
	ea->get_blackboard().set_value<Vec3>("center", ea->get_position());
	ea->get_blackboard().set_value<float>("cooldown", 0.5f);
	ea->set_yaw(-90.0f * 0.0174532925);

	ea = agents->create_behavior_agent("Audience4", BehaviorTreeTypes::Audience);
	ea->set_position(Vec3(104.0f, 0.0f, 70.0f));
	ea->set_color(Vec3(1.0f, 1.0f, 1.0f));
	ea->set_scaling(Vec3(2.0f, 2.0f, 2.0f));
	ea->get_blackboard().set_value<Vec3>("center", ea->get_position());
	ea->get_blackboard().set_value<float>("cooldown", 0.5f);
	ea->set_yaw(-90.0f * 0.0174532925);

	ea = agents->create_behavior_agent("Audience5", BehaviorTreeTypes::Audience);
	ea->set_position(Vec3(104.0f, 0.0f, 90.0f));
	ea->set_color(Vec3(1.0f, 1.0f, 1.0f));
	ea->set_scaling(Vec3(2.0f, 2.0f, 2.0f));
	ea->get_blackboard().set_value<Vec3>("center", ea->get_position());
	ea->get_blackboard().set_value<float>("cooldown", 0.5f);
	ea->set_yaw(-90.0f * 0.0174532925);

	ea = agents->create_behavior_agent("Audience6", BehaviorTreeTypes::Audience);
	ea->set_position(Vec3(114.0f, 0.0f, 10.0f));
	ea->set_color(Vec3(1.0f, 1.0f, 1.0f));
	ea->set_scaling(Vec3(2.0f, 2.0f, 2.0f));
	ea->get_blackboard().set_value<Vec3>("center", ea->get_position());
	ea->get_blackboard().set_value<float>("cooldown", 0.5f);
	ea->set_yaw(-90.0f * 0.0174532925);
	
	ea = agents->create_behavior_agent("Audience7", BehaviorTreeTypes::Audience);
	ea->set_position(Vec3(114.0f, 0.0f, 30.0f));
	ea->set_color(Vec3(1.0f, 1.0f, 1.0f));
	ea->set_scaling(Vec3(2.0f, 2.0f, 2.0f));
	ea->get_blackboard().set_value<Vec3>("center", ea->get_position());
	ea->get_blackboard().set_value<float>("cooldown", 0.5f);
	ea->set_yaw(-90.0f * 0.0174532925);

	ea = agents->create_behavior_agent("Audience8", BehaviorTreeTypes::Audience);
	ea->set_position(Vec3(114.0f, 0.0f, 50.0f));
	ea->set_color(Vec3(1.0f, 1.0f, 1.0f));
	ea->set_scaling(Vec3(2.0f, 2.0f, 2.0f));
	ea->get_blackboard().set_value<Vec3>("center", ea->get_position());
	ea->get_blackboard().set_value<float>("cooldown", 0.5f);
	ea->set_yaw(-90.0f * 0.0174532925);

	ea = agents->create_behavior_agent("Audience9", BehaviorTreeTypes::Audience);
	ea->set_position(Vec3(114.0f, 0.0f, 70.0f));
	ea->set_color(Vec3(1.0f, 1.0f, 1.0f));
	ea->set_scaling(Vec3(2.0f, 2.0f, 2.0f));
	ea->get_blackboard().set_value<Vec3>("center", ea->get_position());
	ea->get_blackboard().set_value<float>("cooldown", 0.5f);
	ea->set_yaw(-90.0f * 0.0174532925);

	ea = agents->create_behavior_agent("Audience10", BehaviorTreeTypes::Audience);
	ea->set_position(Vec3(114.0f, 0.0f, 90.0f));
	ea->set_color(Vec3(1.0f, 1.0f, 1.0f));
	ea->set_scaling(Vec3(2.0f, 2.0f, 2.0f));
	ea->get_blackboard().set_value<Vec3>("center", ea->get_position());
	ea->get_blackboard().set_value<float>("cooldown", 0.5f);
	ea->set_yaw(-90.0f * 0.0174532925);

	ea = agents->create_behavior_agent("Runner1", BehaviorTreeTypes::Runner);
	ea->set_position(Vec3(2.0f, 0.0f, 2.0f));
	ea->set_color(Vec3(2.0f, 2.0f, 0.0f));
	ea->set_scaling(Vec3(2.0f, 2.0f, 2.0f));
	ea->set_movement_speed(25.0f);
	auto &bd2 = ea->get_blackboard();
	bd2.set_value<float>("lane", 2.0f);
	bd2.set_value<float>("cooldown", 3.0f);
	bd2.set_value<Vec3>("Color0", Vec3(2.0f, 0.0f, 0.0f));
	bd2.set_value<Vec3>("Color1", Vec3(2.0f, 2.0f, 0.0f));
	bd2.set_value<int>("colorIndex", 0);

	ea = agents->create_behavior_agent("Runner2", BehaviorTreeTypes::LazyRunner);
	ea->set_position(Vec3(6.0f, 0.0f, 6.0f));
	ea->set_color(Vec3(2.0f, 2.0f, 0.0f));
	ea->set_scaling(Vec3(2.0f, 2.0f, 2.0f));
	ea->set_movement_speed(20.0f);
	auto &bd3 = ea->get_blackboard();
	bd3.set_value<float>("lane", 6.0f);
	bd3.set_value<float>("cooldown", 2.0f);
	bd3.set_value<Vec3>("Color0", Vec3(0.0f, 2.0f, 0.0f));
	bd3.set_value<Vec3>("Color1", Vec3(2.0f, 2.0f, 0.0f));
	bd3.set_value<int>("colorIndex", 0);

	ea = agents->create_behavior_agent("Runner3", BehaviorTreeTypes::Runner);
	ea->set_position(Vec3(20.0f, 0.0f, 20.0f));
	ea->set_color(Vec3(2.0f, 2.0f, 0.0f));
	ea->set_scaling(Vec3(2.0f, 2.0f, 2.0f));
	ea->set_movement_speed(25.0f);
	auto &bd4 = ea->get_blackboard();
	bd4.set_value<float>("lane", 20.0f);
	bd4.set_value<float>("cooldown", 3.0f);
	bd4.set_value<Vec3>("Color0", Vec3(2.0f, 0.0f, 0.0f));
	bd4.set_value<Vec3>("Color1", Vec3(2.0f, 2.0f, 0.0f));
	bd4.set_value<int>("colorIndex", 0);

	ea = agents->create_behavior_agent("Wanderer1", BehaviorTreeTypes::Wanderer);
	ea->set_position(Vec3(40.0f, 0.0f, 40.0f));
	ea->set_color(Vec3(0.0f, 2.0f, 2.0f));
	ea->set_scaling(Vec3(2.0f, 2.0f, 2.0f));
	ea->set_movement_speed(15.0f);
	ea->get_blackboard().set_value<Vec3>("targetPoint", Vec3(50.0f, 0.0f, 50.0f));

	ea = agents->create_behavior_agent("Wanderer2", BehaviorTreeTypes::Wanderer);
	ea->set_position(Vec3(60.0f, 0.0f, 60.0f));
	ea->set_color(Vec3(2.0f, 0.0f, 2.0f));
	ea->set_scaling(Vec3(2.0f, 2.0f, 2.0f));
	ea->set_movement_speed(15.0f);
	ea->get_blackboard().set_value<Vec3>("targetPoint", Vec3(50.0f, 0.0f, 50.0f));

	for (int i = 0; i < 25; ++i)
	{
		ea = agents->create_behavior_agent("Boid", BehaviorTreeTypes::Boid);
		float f = 3.0f + 0.1f * (float)i;
		ea->set_position(Vec3(RNG::range(10.0f, 90.0f), 0.0f, RNG::range(10.0f, 90.0f)));
		ea->set_yaw(RNG::range(0.0f, 6.2831853072f));
		ea->set_color(Vec3(2.0f, 0.0f, 2.0f));
		ea->set_scaling(Vec3(2.0f, 0.5f, 2.0f));
		ea->set_movement_speed(10.0f);
		ea->get_blackboard().set_value<Vec3>("targetPoint", Vec3(50.0f, 0.0f, 50.0f));
	}

	/*ea = agents->create_behavior_agent("ExampleAgent", BehaviorTreeTypes::Example);
	ea->set_position(Vec3(50.0f, 0.0f,50.0f));
	ea->set_color(Vec3(10.0f, 0.0f, 0.0f));*/
    // you can technically load any map you want, even create your own map file,
    // but behavior agents won't actually avoid walls or anything special, unless you code that yourself
    // that's the realm of project 2 though
    terrain->goto_map(0);

    // you can also enable the pathing layer and set grid square colors as you see fit
    // works best with map 0, the completely blank map
    terrain->pathLayer.set_enabled(true);
    terrain->pathLayer.set_value(0, 0, Colors::Red);

    // camera position can be modified from this default as well
    auto camera = agents->get_camera_agent();
    camera->set_position(Vec3(-62.0f, 70.0f, terrain->mapSizeInWorld * 0.5f));
    camera->set_pitch(0.610865); // 35 degrees
}