#pragma once

#include "Agent.h"

class Updater
{
public:
	Updater();
	~Updater();

	void update(Agent &agent, TargetTrans target, std::vector<Agent>, std::vector<Obstacle>, float time);
};

