#pragma once

#include "stdafx.h"
#include "Global.h"
#include "behavior.h"

enum state
{
	obstacley,
	collisionfovy,
	collisionpredicty,
	separationy,
	seeky,
	fleey,
	arrivey,
	wandery,
	followpathy,
	pursuey,
	evadey
};

struct Blending
{
	state st;
	int priority;
	int weight;
};

class Agent
{
public:
	Agent(float max_vel, float max_acc, float max_angvel, float max_angacc);
	Agent();
	~Agent();

	//Transform SelectBehavior(TargetTrans target, Agent &agent);

	inline Vector2D getposition() { return position; };
	inline Orientation getorientation() { return orientation; };

	inline Vector2D getvelocity() { return velocity; };
	inline float getangularvelocity() { return angularvelocity; };

	inline float getmaxvelocity() { return max_velocity; };
	inline float getmaxacceleration() { return max_acceleration; };
	inline float getmaxangvelocity() { return max_angvelocity; };
	inline float getmaxangacceleration() { return max_angacceleration; };

	inline void setpostion(Vector2D pos) { position = pos; };
	inline void setorientation(Orientation orien) { orientation = orien; };
	inline void setvelocity(Vector2D vel) { velocity = vel; };
	inline void setangularvelocity(float avel) { angularvelocity = avel; };

	inline float getwanderOffset()	{ return wanderOffset; };
	inline float getwanderRadius()	{ return wanderRadius; };
	inline float getwanderRate()	{ return wanderRate; };

	inline std::vector<Vector2D> getPath()			{ return Path; };
	inline int getPathOffset()					{ return pathOffset; };
	inline void setPath(std::vector<Vector2D> aPath){ Path = aPath; };

	inline float getseparationRadius() { return separationRadius; };

	inline void setState(state st, int priority, int weight)	{
		Blending temp;
		temp.st = st;
		temp.priority = priority;
		temp.weight = weight;
		Group.pop_back();
		Group.push_back(temp);
	 };
	inline Blending getState() { return Group[Group.size()-1]; };

	inline std::vector<Blending> getGroup() { return Group; };
	inline void setGroup(state st, int priority, int weight) {  
		Blending temp;
		temp.st = st;
		temp.priority = priority;
		temp.weight = weight;
		Group.push_back(temp); };

	inline TargetTrans asTarget() {
		TargetTrans target;
		target.angvelocity = this->getangularvelocity();
		target.orientation = this->getorientation();
		target.position = this->getposition();
		target.velocity = this->getvelocity();

		return target;
	};

	inline float getFOV() { return FOVangle; };

	Vector2D collision;

private:

	Behavior behave;

	Vector2D position;
	Orientation orientation;
	Vector2D velocity;
	float angularvelocity;

	float max_velocity;
	float max_acceleration;

	float max_angvelocity;
	float max_angacceleration;

	float wanderOffset = 10.f;
	float wanderRadius = 20.f;
	float wanderRate = 10.f;

	float separationRadius = 50.f;
	
	float FOVangle = 3.1415926f / 4;

	int pathOffset = 1;

	std::vector<Vector2D> Path;
	std::vector<Blending> Group;

	state State = seeky;
};

