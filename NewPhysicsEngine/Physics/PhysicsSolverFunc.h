#pragma once

#include "../ProjectInclude.h"

#include "Entity.h"

// Info wrapper

struct CollisionTestInfo
{
	EntityData::Geom *a, *b;
	CollisionTestInfo(EntityData::Geom* a, EntityData::Geom* b)
	{
		this->a = a;
		this->b = b;
	}
};
struct CollisionInfo
{
	int collisionType;
	EntityData::Geom *a, *b;
	Vector2 normal;
	float depth;
	bool isMovingTowardEachOther;
	CollisionInfo(int collisionType, EntityData::Geom* a, EntityData::Geom* b, Vector2 normal, float depth, bool isMovingTowardEachOther)
	{
		this->a = a;
		this->b = b;
		this->normal = normal;
		this->depth = depth;
		this->isMovingTowardEachOther = isMovingTowardEachOther;
		this->collisionType = collisionType;
	}

	CollisionInfo()
	{

	}
};
struct CollisionWrapper
{
	CollisionInfo info;
	bool isCollide;
	
	CollisionWrapper(bool isCollide, CollisionInfo info)
	{
		this->isCollide = isCollide;
		this->info = info;
	}

	operator bool()
	{
		return isCollide;
	}
};

// Narrow-phase methods //////////////////////////////////

const CollisionWrapper Detect_CircleCircle(Entity* a, Entity* b)
{
	return CollisionWrapper
	(
		false,
		CollisionInfo
		(

		)
	);
}

const CollisionWrapper Detect_CirclePlane(EntityData::Geom* circle, EntityData::Geom* plane)
{
	Vector2 planeNormal = plane->props.plane.normal;
	Vector2 planePos = plane->COM;
	Vector2 closestPointToPlane = circle->COM - planeNormal * circle->props.circle.radius;

	float dist = (planeNormal.x * (planePos.y - planeNormal.y) - planeNormal.y * (planePos.x - planeNormal.x));

	return CollisionWrapper
	(
		dist > 0,
		CollisionInfo
		(
			
		)
	);
}

const std::function<CollisionWrapper(EntityData::Geom, EntityData::Geom)> detectFuncList[]
{
	&Detect_CircleCircle,
	&Detect_CirclePlane
};

const std::function<CollisionWrapper(EntityData::Geom, EntityData::Geom)> DetectFunc(int i)
{
	return detectFuncList[i];
}



// Detect methods //////////////////////////////////

const void Solve_CircleCircle(CollisionWrapper c)
{
	float massA = a.GetMass();
	float massB = b.GetMass();

	Vector2 velA = a.geom.vel;
	Vector2 velB = b.geom.vel;

	float minRestitution = std::min(a.geom.restitution, a.geom.restitution);
	float totalMass = massA + massB;

	a.geom.vel = (velA * (massA - massB) + velB * massB * 2) / totalMass * minRestitution;
	b.geom.vel = (velB * (massB - massA) + velA * massA * 2) / totalMass * minRestitution;
}

const void Solve_CirclePlane(CollisionWrapper c)
{
	
}

const std::function<void(CollisionWrapper)> solveFuncList[]
{
	&Solve_CircleCircle,
	&Solve_CirclePlane
};

const std::function<void(CollisionWrapper)> SolveFunc(int i)
{
	return solveFuncList[i];
}