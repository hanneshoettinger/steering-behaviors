#pragma once

#include "stdafx.h"
#include "Global.h"
class Agent;

class Behavior
{
public:
	Behavior();
	~Behavior();

	Transform SelectBehavior(TargetTrans target, Agent & agent, std::vector<Agent> agents, std::vector<Obstacle> obstacles);

	Transform Seek(Vector2D targetposition, Agent & agent);

	Transform Arrive(Vector2D targetposition, Agent & agent);

	float Align(Vector2D targetorientation, Agent & agent);

	Transform Pursue(TargetTrans target, Agent & agent);

	Transform FollowPath(Agent & agent);

	Transform Wander(Agent & agent);

	Transform Separation(Agent & agent, std::vector<Agent> neighbours);

	Transform CollisionFOV(Agent & agent, std::vector<Agent> neighbours);

	Transform CollisionPrediction(Agent & agent, std::vector<Agent> neighbours);

	Transform ObstacleAvoidance(Agent & agent, std::vector<Obstacle> obstacles);

private:
	
	float randomBinomial();

	Vector2D RayBoxIntersect(Obstacle b, Vector2D origin, Vector2D rayDirection);

	Vector2D getNormal(Obstacle obstacle, Vector2D collision);

	float wanderOrientation = 0.f;

	float maxPrediction = 5.5f;

	float k = 30.0f;

	std::vector<char> group;

	float epsilon = 0.1f;

};

