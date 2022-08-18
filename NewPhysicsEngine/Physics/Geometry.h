#pragma once

#include "Entity.h"

class Geometry : public Entity
{
private:
	float mass;
	
public:
	
	// Physics properties
	Vector2 vel;
	float angvel = 0;

	float density = 1;
	float friction = 0;
	float restitution = 0;

	unsigned short collisionLayers = 0b01;


	Geometry(Transform2D transform, EntityType type) : Entity(transform, type)
	{
		CalculateAABB();
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
};