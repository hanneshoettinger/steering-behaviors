#include "stdafx.h"
#include "Updater.h"
#include "formation.h"
#include "Global.h"

#define PI 3.14159265


Updater::Updater()
{
}


Updater::~Updater()
{
}

void Updater::update(Agent &agent, TargetTrans target, std::vector<Agent> agents, std::vector<Obstacle> obstacles, float time)
{
	Transform trans;
	//trans = agent.SelectBehavior(target, agent);
	Behavior behave;
	trans = behave.SelectBehavior(target, agent, agents, obstacles);

	Vector2D position = agent.getposition();
	Vector2D velocity = agent.getvelocity();
	float angvelocity = agent.getangularvelocity();
	Orientation orientation = agent.getorientation();

	position += velocity*time;
	agent.setpostion(position);

	orientation.angle += angvelocity*time;
	agent.setorientation(orientation);

	// check again the velocity -> arrive can set it to zero
	//velocity = agent.getvelocity();
	velocity += trans.acceleration * time;
	if (velocity.getLength() > agent.getmaxvelocity())
	{
		velocity.normalize();
		velocity *= agent.getmaxvelocity();
	}
	agent.setvelocity(velocity);

	angvelocity += trans.angacceleration * time;
	if (angvelocity > agent.getmaxangvelocity())
	{
		angvelocity = agent.getmaxangvelocity();
	}
	agent.setangularvelocity(angvelocity);

}

