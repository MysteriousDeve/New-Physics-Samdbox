#pragma once

#include "Entity.h"

class Geometry : public Entity
{
private:
	float mass;
	float area = -1;
public:
	
	// Physics properties
	Vector2 vel = Vector2(0, 0);
	float angvel = 0;

	// Variables for Verlet integration
	Vector2 lastPos;
	Vector2 accel;

	float density = 1;
	float friction = 0;
	float restitution = 0;

	unsigned short collisionLayers = 0b01;


	Geometry(Transform2D transform, EntityType type) : Entity(transform, type)
	{
		CalculateAABB();
		//ResetVelocity();
	}

	// AABB stuffs
	AABB boundingBox;
	void CalculateAABB()
	{
		if (entityType == EntityType::Circle)
		{
			boundingBox = AABB(transform.position, transform.scale.Max());
		}
	}
	void AssignAABBPosition()
	{
		boundingBox.pos = transform.position;
	}

	float GetArea()
	{
		if (area < 0)
		{
			RecalculateArea();
		}
		return area;
	}

	void RecalculateArea()
	{
		switch (entityType)
		{
			case EntityType::Circle: area = pow(transform.scale.x / 2.0, 2) * 3.14159265f;
			default: area = 1;
		}
	}

	float GetMass()
	{
		return density * GetArea();
	}

	void SetVelocity(Vector2 vel)
	{
		/*lastPos = transform.position - vel;*/
		this->vel = vel;
	}

	Vector2 GetVelocity()
	{
		/*return transform.position - lastPos;*/
		return vel;
	}
};