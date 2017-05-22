// unitTest_Pacman.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "gtest/gtest.h"

// Include the header files for the Unit Test

#include "Agent.h"
#include "behavior.h"
#include "Updater.h"

// steering tests

TEST(TestSteering, Seek)
{
	Agent agent(30, 10, 5, 2);
	agent.setGroup(seeky, 1, 1);
	Updater kinematic;
	std::vector<Obstacle> obstacle;
	std::vector<Agent> agents;

	TargetTrans targetStruct;
	targetStruct.angvelocity = 0.f;
	targetStruct.orientation.angle = 0.f;
	targetStruct.velocity = Vector2D(20, 0);
	targetStruct.position = Vector2D(40, 600);

	kinematic.update(agent, targetStruct, agents, obstacle, 1.f);
	kinematic.update(agent, targetStruct, agents, obstacle, 1.f);

	EXPECT_NE(0.f, agent.getposition().x);
	EXPECT_NE(0.f, agent.getposition().y);
}

TEST(TestSteering, Evade)
{
	Agent agent(30, 10, 5, 2);
	agent.setGroup(evadey, 1, 1);
	Updater kinematic;
	std::vector<Obstacle> obstacle;
	std::vector<Agent> agents;

	// agent pos left from target -> evade should decrease at least x pos
	agent.setpostion(Vector2D(30, 600));

	TargetTrans targetStruct;
	targetStruct.angvelocity = 0.f;
	targetStruct.orientation.angle = 0.f;
	targetStruct.velocity = Vector2D(20, 0);
	targetStruct.position = Vector2D(40, 600);

	kinematic.update(agent, targetStruct, agents, obstacle, 1.f);
	kinematic.update(agent, targetStruct, agents, obstacle, 1.f);

	EXPECT_LT(agent.getposition().x, 30.f);
}


TEST(TestSteering, Arrive)
{
	Agent agent(30, 10, 5, 2);
	agent.setGroup(arrivey, 1, 1);
	Updater kinematic;
	std::vector<Obstacle> obstacle;
	std::vector<Agent> agents;

	// agent is inside the slowRadius -> no velocity
	agent.setpostion(Vector2D(30, 600));

	TargetTrans targetStruct;
	targetStruct.angvelocity = 0.f;
	targetStruct.orientation.angle = 0.f;
	targetStruct.velocity = Vector2D(20, 0);
	targetStruct.position = Vector2D(40, 600);

	kinematic.update(agent, targetStruct, agents, obstacle, 1.f);
	kinematic.update(agent, targetStruct, agents, obstacle, 1.f);

	EXPECT_EQ(Vector2D(0, 0), agent.getvelocity());

	// agent is outside the radius -> velocity
	agent.setpostion(Vector2D(30, 300));

	kinematic.update(agent, targetStruct, agents, obstacle, 1.f);
	kinematic.update(agent, targetStruct, agents, obstacle, 1.f);

	EXPECT_LT(0.f, agent.getvelocity().x);
	EXPECT_LT(0.f, agent.getvelocity().y);
}

