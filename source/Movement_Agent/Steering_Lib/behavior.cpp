#include "stdafx.h"
#include "behavior.h"
#include "Agent.h"

#include <math.h>

#include <iostream>
#include "Global.h"


Behavior::Behavior()
{
}


Behavior::~Behavior()
{
}

Transform Behavior::SelectBehavior(TargetTrans target, Agent &agent, std::vector<Agent> agents, std::vector<Obstacle> obstacles)
{
	Transform trans, temp;
	temp.acceleration = Vector2D(0.f, 0.f);
	temp.angacceleration = 0.f;

	for each(Blending c in agent.getGroup())
	{
		switch (c.st)
		{
		case seeky: // seek
			trans = Seek(target.position, agent);
			break;
		case fleey: // flee
			trans = Seek(target.position, agent);
			trans.acceleration = trans.acceleration * -1;
			break;
		case arrivey: // arrive
			trans = Arrive(target.position, agent);
			break;
		case wandery: // wander
			trans = Wander(agent);
			break;
		case pursuey:  // pursue
			trans = Pursue(target, agent);
			break;
		case evadey:  // evade
			trans = Pursue(target, agent);
			trans.acceleration = trans.acceleration * -1;
			break;
		case followpathy:  // follow Path
			trans = FollowPath(agent);
			break;
		case obstacley:  // obstacleavoid
			trans = ObstacleAvoidance(agent, obstacles);
			break;
		case collisionfovy:  // collisionavoid FOV
			trans = CollisionFOV(agent, agents);
			break;
		case collisionpredicty:  // collisionavoid Predict
			trans = CollisionPrediction(agent, agents);
			break;
		case separationy:  // separation
			trans = Separation(agent, agents);
			break;
		default:
			break;
		}

		if (c.priority != 3 && (temp.acceleration.getLength() > epsilon || fabsf(temp.angacceleration) > epsilon))
		{
			return temp;
		}
		if (trans.acceleration.getLength() > epsilon || fabsf(trans.angacceleration) > epsilon)
		{
			if (c.priority != 3)
			{
				return trans;
			}
			else
			{
				temp.acceleration += trans.acceleration * c.weight;
				temp.angacceleration += trans.angacceleration * c.weight;
			}
		}
	}

	return trans;
}

Transform Behavior::Seek(Vector2D targetposition, Agent &agent)
{
	Transform temp;

	Vector2D vel = targetposition - agent.getposition();

	vel.normalize();

	temp.acceleration = vel * agent.getmaxacceleration();

	temp.angacceleration = Align(agent.getvelocity(), agent);

	return temp;
}

Transform Behavior::Arrive(Vector2D targetposition, Agent & agent)
{
	Transform temp;

	float timeToTarget = 0.1f;
	float targetRadius = 40.f;
	float slowRadius = 120.f;

	// calculate direction
	Vector2D ToTarget = targetposition - agent.getposition();
	//calculate the distance to the target position
	float dist = ToTarget.getLength();
	float targetSpeed;

	//std::cout << dist << std::endl;

	// if in slow Radius stopp
	if (dist < targetRadius)
	{
		agent.setvelocity(Vector2D(0, 0));
		agent.setangularvelocity(0.f);
		temp.acceleration = Vector2D(0, 0);
		return temp;
	}
	// if outside target Radius -> full velocity
	if (dist > slowRadius)
	{
		targetSpeed = agent.getmaxvelocity();
	}
	// if inside -> break
	else
	{
		//std::cout << (agent.getmaxvelocity() * agent.getvelocity().getLength()) << std::endl;
		targetSpeed = (agent.getmaxvelocity() * dist) / slowRadius;
	}

	// Normalize direction and calculate final target speed
	ToTarget.normalize();
	Vector2D agentvelocity = ToTarget * targetSpeed;

	// steering acceleration
	Vector2D acc = (agentvelocity - agent.getvelocity()) / timeToTarget;
	// set to max acc if it exceeds
	if (acc.getLength() > agent.getmaxacceleration())
	{
		acc.normalize();
		acc *= agent.getmaxacceleration();
	}

	temp.acceleration = acc;
	//std::cout << targetSpeed << std::endl;

	temp.angacceleration = Align(agent.getvelocity(), agent);

	return temp;

}


float Behavior::Align(Vector2D targetorientation, Agent & agent)
{
	//****************** Angular Calculation - Align *****************************
	float angacceleration = 0.f;

	float targetRotation = 0.f;
	float satisfactionAngle = 0.1f;
	float slowAngle = 2.f;
	float timeToTarget = 0.1f;

	float targetangle = atan2f(-targetorientation.x, targetorientation.y);

	float DeltaAngle = targetangle - agent.getorientation().angle;

	float rotation = DeltaAngle - 2 * M_PI * floor((DeltaAngle + M_PI) / (2 * M_PI));
	//float rotation = fmodf(DeltaAngle, 2.0 * M_PI) - M_PI;

	float rotationSize = fabs(rotation);

	if (rotationSize < satisfactionAngle)
	{
		angacceleration = 0;
		agent.setangularvelocity(0.f);
		return angacceleration;
	}
	if (rotationSize > slowAngle)
	{
		targetRotation = agent.getmaxangvelocity();
	}
	else
	{
		targetRotation = agent.getmaxangvelocity() * (rotationSize / slowAngle);
	}

	targetRotation *= (rotation / rotationSize);

	angacceleration = targetRotation - agent.getangularvelocity();
	angacceleration /= timeToTarget;

	float angAcc = fabs(angacceleration);

	if (angAcc > agent.getmaxangacceleration())
	{
		angacceleration /= angAcc;
		angacceleration *= agent.getmaxangacceleration();
	}

	return angacceleration;
}

Transform Behavior::Pursue(TargetTrans target, Agent & agent)
{
	Transform temp;

	float prediction = 0.f;

	Vector2D direction = target.position - agent.getposition();
	float distance = direction.getLength();

	float speed = agent.getvelocity().getLength();

	if (speed <= distance / maxPrediction)
	{
		prediction = maxPrediction;
	}
	else
	{
		prediction = distance / speed;
	}

	target.position += target.velocity * prediction;

	temp = Seek(target.position, agent);

	return temp;
}

Transform Behavior::FollowPath(Agent & agent)
{
	int i = 0, index = 0;
	float dist = std::numeric_limits<float>::max();
	
	for each (Vector2D vect in agent.getPath())
	{
		Vector2D temp = (vect - agent.getposition());
		if (fabs(temp.getLength()) < dist)
		{
			index = i;
			dist = fabs(temp.getLength());
		}
		i++;
	}

	Transform temp;

	// arrive on last pos
	if (index == agent.getPath().size()-1)
	{
		temp = Arrive(agent.getPath()[index], agent);
		return temp;
	}

	index += agent.getPathOffset();
	//index = (index) % (agent.getPath().size());

	temp = Seek(agent.getPath()[index], agent);

	return temp;
}

Transform Behavior::Wander(Agent & agent)
{
	Transform temp;
	Orientation targetOrientation;
	targetOrientation.angle = 0;
	Vector2D target;

	wanderOrientation += randomBinomial() * agent.getwanderRate();

	targetOrientation = wanderOrientation + agent.getorientation().angle;

	target = agent.getposition() + agent.getorientation().asVector() * agent.getwanderOffset();

	target += targetOrientation.asVector() * agent.getwanderRadius();

	Vector2D direction = target - agent.getposition();

	if (direction.getLength() == 0)
	{
		temp.angacceleration = targetOrientation.angle;
	}
	else
	{
		temp.angacceleration = atan2f(-direction.x, direction.y);
	}

	temp.acceleration = agent.getorientation().asVector() * agent.getmaxacceleration();

	return temp;
}

Transform Behavior::Separation(Agent & agent, std::vector<Agent> neighbours)
{
	Transform trans;
	Vector2D direction;
	float distance, strength;

	for each(Agent nb in neighbours)
	{
		direction = nb.getposition() - agent.getposition();
		distance = direction.getLength();
		if (distance > 0.f && distance < agent.getseparationRadius())
		{
			strength = fminf(k * (1.0f / (distance * distance)), agent.getmaxacceleration());
			direction.normalize();
			trans.acceleration += (direction * strength) * (-1);
			trans.angacceleration = Align(agent.getvelocity(), agent);
		}
	}

	//trans.angacceleration = Align(agent.getvelocity(), agent);
	return trans;
}

Transform Behavior::CollisionFOV(Agent & agent, std::vector<Agent> neighbours)
{
	Transform trans;
	Vector2D direction;

	Vector2D orientation = agent.getorientation().asVector();

	for each(Agent nb in neighbours)
	{
		direction = nb.getposition() - agent.getposition();

		if (direction.getLength() != 0.f)  // check for check the agent itself
		{
			if (fabs(orientation.dot(direction)) > agent.getFOV()  && direction.getLength() < agent.getseparationRadius())
			//if (direction.dot(nb.getorientation().asVector()) > agent.getFOV() && direction.getLength() < agent.getseparationRadius())
			{
				trans = Pursue(nb.asTarget(), agent);
				trans.acceleration = trans.acceleration * -1;
				return trans;
			}
		}
	}

	return trans;
}

Transform Behavior::CollisionPrediction(Agent & agent, std::vector<Agent> neighbours)
{
	Transform trans;
	Vector2D direction, directionAgent;
	Vector2D veldiff, veldiffAgent;
	Agent collAgent;
	float tclosest = std::numeric_limits<float>::max();
	float dmin, dminAgent;

	for each(Agent nb in neighbours)
	{
		direction = nb.getposition() - agent.getposition();
		veldiff = nb.getvelocity() - agent.getvelocity();

		if ((veldiff.getLength() > epsilon))
		{
			float tclose = (-1) * ( direction.dot(veldiff)) / (veldiff.getLength());

			//if (tclose <= 0 || tclose >= tclosest) continue;

			dmin = direction.getLength() - veldiff.getLength() * tclose;

			if (fabs(dmin) < 2 * agent.getseparationRadius() && tclose < tclosest)
			{
				tclosest = tclose;
				collAgent = nb;
				dminAgent = dmin;
				directionAgent = direction;
				veldiffAgent = veldiff;
			}
		}
	}

	if (tclosest == std::numeric_limits<float>::max())
	{
		trans.acceleration = Vector2D(0.f, 0.f);
		trans.angacceleration = 0.f;
		return trans;
	}

	if (dminAgent <= 0.f || directionAgent.getLength() < 2 * agent.getseparationRadius())
	{
		trans = Pursue(collAgent.asTarget(), agent);
		trans.acceleration = trans.acceleration * -1;
	}
	else
	{
		Vector2D dk = directionAgent + veldiffAgent * tclosest;
		dk.normalize();
		trans.angacceleration = Align(agent.getvelocity(), agent);
		trans.acceleration = dk * agent.getmaxacceleration() * (-1);
	}

	return trans;
}
/*
Transform Behavior::CollisionPrediction(Agent & agent, std::vector<Agent> neighbours)
{
	Transform trans;
	Vector2D relativePos, directionAgent, veldiffAgent;
	Vector2D relativeVel;
	float relativeSpeed, distance;
	float minSeparation = std::numeric_limits<float>::max();
	float separationAgent = std::numeric_limits<float>::max();
	float shortestTime = std::numeric_limits<float>::max();
	float timeToCollision;
	Agent collAgent;
	
	float dmin, dminAgent;

	for each(Agent nb in neighbours)
	{
		relativePos = nb.getposition() - agent.getposition();
		relativeVel = nb.getvelocity() - agent.getvelocity();

		relativeSpeed = relativeVel.getLength();

		if (relativeSpeed <= epsilon) continue;

		timeToCollision = (relativePos.dot(relativeVel)) / (relativeSpeed * relativeSpeed);

		distance = relativePos.getLength();

		minSeparation = distance - relativeSpeed * shortestTime;

		if (minSeparation > 2 * agent.getseparationRadius()) { continue; }

		if (timeToCollision > 0 && timeToCollision < shortestTime)
		{
			shortestTime = timeToCollision;
			collAgent = nb;
			dminAgent = distance;
			directionAgent = relativePos;
			veldiffAgent = relativeVel;
			separationAgent = minSeparation;
		}
	}

	// if no target return
	if (shortestTime == std::numeric_limits<float>::max())
	{
		trans.acceleration = Vector2D(0.f, 0.f);
		trans.angacceleration = 0.f;
		return trans;
	}

	if (separationAgent <= 0.f || dminAgent < 2 * agent.getseparationRadius())
	{
		relativePos = directionAgent - agent.getposition();
	}
	else
	{
		relativePos = directionAgent + veldiffAgent * shortestTime;
	}

	relativePos.normalize();
	trans.acceleration = relativePos * agent.getmaxacceleration();
	trans.angacceleration = Align(agent.getvelocity(), agent);

	return trans;
}
*/
Transform Behavior::ObstacleAvoidance(Agent & agent, std::vector<Obstacle> obstacles)
{
	Transform trans;
	trans.acceleration = Vector2D(0.f, 0.f);
	trans.angacceleration = 0.f;
	float lookAhead = 80.f;
	float lookAheadW = 30.f;
	float avoidDistance = 100.f;
	Vector2D rayVector, collNormal;
	Vector2D whiskerLeft, whiskerRight;
	rayVector = agent.getvelocity();
	rayVector.normalize();
	rayVector *= lookAhead;

	agent.collision = Vector2D(0.f, 0.f);

	whiskerLeft = agent.getvelocity();
	whiskerLeft.rotateBy(15 * M_PI / 180);
	whiskerLeft.normalize();
	whiskerLeft *= lookAheadW;

	whiskerRight = agent.getvelocity();
	whiskerRight.rotateBy(-15 * M_PI / 180);
	whiskerRight.normalize();
	whiskerRight *= lookAheadW;

	for each(Obstacle obstacle in obstacles)
	{
		Vector2D collision = RayBoxIntersect(obstacle, agent.getposition(), rayVector);
		
		Vector2D collDiff = collision - agent.getposition();
		if (collision.x < std::numeric_limits<float>::max()-1 && collision.y < std::numeric_limits<float>::max()-1 && collDiff.getLength() < lookAhead)
		{	
			
			collNormal = getNormal(obstacle, collision);

			Vector2D target = collision + collNormal * avoidDistance;

			trans = Seek(target, agent);
			trans.acceleration *= 2;
			
			agent.collision = collision;

			return trans;
		}
		collision = RayBoxIntersect(obstacle, agent.getposition(), whiskerLeft);

		collDiff = collision - agent.getposition();

		if (collision.x < std::numeric_limits<float>::max() - 1 && collision.y < std::numeric_limits<float>::max() - 1 && collDiff.getLength() < lookAheadW)
		{
			///collNormal = getNormal(obstacle, collision);

			Vector2D target = collision + whiskerRight * avoidDistance;

			Transform temp = Seek(target, agent);
			temp.acceleration += trans.acceleration;

			agent.collision = collision;

			return temp;
		}

		collision = RayBoxIntersect(obstacle, agent.getposition(), whiskerRight);

		collDiff = collision - agent.getposition();

		if (collision.x < std::numeric_limits<float>::max() - 1 && collision.y < std::numeric_limits<float>::max() - 1 && collDiff.getLength() < lookAheadW)
		{
			//collNormal = getNormal(obstacle, collision);

			Vector2D target = collision + whiskerLeft * avoidDistance;

			Transform temp = Seek(target, agent);
			temp.acceleration += trans.acceleration;

			agent.collision = collision;

			return temp;
		}
		
	}
	
	return trans;
}

float Behavior::randomBinomial()
{
	float LO = -1.0f;
	float HI = 1.0f;
	// binomial random rumbers in range
	float result = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
	//float result = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	result *= result;

	return result;
}

Vector2D Behavior::RayBoxIntersect(Obstacle b, Vector2D origin, Vector2D rayDirection) 
{
	float tmin = -std::numeric_limits<float>::max(), tmax = std::numeric_limits<float>::max();
	Vector2D intersectPos(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());

	if (rayDirection.x != 0.0) {
		float tx1 = (b.points[0].x - origin.x) / rayDirection.x;
		float tx2 = (b.points[1].x - origin.x) / rayDirection.x;

		tmin = fmaxf(tmin, fminf(tx1, tx2));
		tmax = fminf(tmax, fmaxf(tx1, tx2));
	}

	if (rayDirection.y != 0.0) {
		float ty1 = (b.points[0].y - origin.y) / rayDirection.y;
		float ty2 = (b.points[1].y - origin.y) / rayDirection.y;

		tmin = fmaxf(tmin, fminf(ty1, ty2));
		tmax = fminf(tmax, fmaxf(ty1, ty2));
	}

	if (tmax >= tmin)
	{
		//intersection
		intersectPos = origin + rayDirection * tmin;
	}

	return intersectPos;
}

Vector2D Behavior::getNormal(Obstacle obstacle, Vector2D collision)
{
	Vector2D collNormal;

	if (obstacle.points[0].y <= collision.y + epsilon && obstacle.points[0].y > collision.y - epsilon)
	{
		collNormal = Vector2D(0, -1);
	}
	else if (obstacle.points[1].y <= collision.y + epsilon && obstacle.points[1].y > collision.y - epsilon)
	{
		collNormal = Vector2D(0, 1);
	}
	else if (obstacle.points[0].x <= collision.x + epsilon && obstacle.points[0].x > collision.x - epsilon)
	{
		collNormal = Vector2D(-1, 0);
	}
	else if (obstacle.points[1].x <= collision.x + epsilon && obstacle.points[1].x > collision.x - epsilon)
	{
		collNormal = Vector2D(1, 0);
	}
	return collNormal;
}