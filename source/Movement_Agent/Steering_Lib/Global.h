#pragma once
#include "stdafx.h"

#define M_PI 3.14159265

struct Transform
	{
		Vector2D acceleration;
		float angacceleration = 0;

		inline void operator+=(const Transform & rhs)
		{
			this->acceleration += rhs.acceleration;
			this->angacceleration += rhs.angacceleration;
		}
	};


struct Orientation
{
	float angle;

	inline Vector2D asVector()
	{
		Vector2D temp;
		temp.x = cos(angle);
		temp.y = sin(angle);
		return temp;
	};
	
	inline void operator=(const float rhs)
	{
		this->angle = rhs;
	}
};

struct TargetTrans
{
	Vector2D position;
	Vector2D velocity;
	Orientation	orientation;
	float angvelocity;
};

struct Obstacle
{
	Vector2D points[2];
	Vector2D center;
	Orientation orientation;

	Obstacle::Obstacle(Vector2D dim, Vector2D pos)
	{
		points[0] = Vector2D(pos.x - dim.x / 2, pos.y - dim.y / 2);// xmin ymin
 		points[1] = Vector2D(pos.x + dim.x / 2, pos.y + dim.y / 2);	// xmax ymax
		center = pos;
	}
};