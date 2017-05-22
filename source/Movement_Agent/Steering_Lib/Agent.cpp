#include "stdafx.h"
#include "Agent.h"


Agent::Agent(float max_vel, float max_acc, float max_angvel, float max_angacc)
{
	max_velocity = max_vel;
	max_acceleration = max_acc;

	max_angvelocity = max_angvel;
	max_angacceleration = max_angacc;

	orientation.angle = 0.f;
}

Agent::Agent()
{
}


Agent::~Agent()
{
}
/*
Transform Agent::SelectBehavior(TargetTrans target, Agent &agent)
{
	Transform trans;

	switch (State)
	{
	case 's': // seek
		trans = behave.Seek(target.position, agent);
		break;
	case 'f': // flee
		trans = behave.Seek(target.position, agent);
		trans.acceleration = trans.acceleration * -1;
		break;
	case 'a': // arrive
		trans = behave.Arrive(target.position, agent);
		break;
	case 'w': // wander
		trans = behave.Wander(agent);
		break;
	case 'p':  // pursue
		trans = behave.Pursue(target, agent);
		break;
	case 'e':  // evade
		trans = behave.Pursue(target, agent);
		trans.acceleration = trans.acceleration * -1;
		break;
	case 'b':  // evade
		trans = behave.FollowPath(agent);
		break;
	default:
		break;
	}

	return trans;
}
*/