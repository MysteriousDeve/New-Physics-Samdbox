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

int GetGeometryCollisionId(EntityType t0, EntityType t1)
{
	int n0 = t0 & 1, n1 = t1 & 1;
	if (n1 > n0) swap(n0, n1);
	return n0 * 4 + n1;
}

// Narrow-phase methods //////////////////////////////////
#define DETECT_PARAM(name0, name1, collisionId) EntityData::Geom* name0, EntityData::Geom* name1, int collisionId
#define DETECT_PARAM_TYPE DETECT_PARAM(,,)

const CollisionWrapper Detect_CircleCircle(DETECT_PARAM(c0, c1, collisionId))
{
	Vector2 distVec = c0->transform.position - c1->transform.position;
	float dist = distVec.len() - c0->props.circle.radius + c1->props.circle.radius;
	return CollisionWrapper
	(
		dist > 0,
		CollisionInfo
		(
			collisionId, c0, c1, distVec.Normalize(), dist, false // idk
		)
	);
}

const CollisionWrapper Detect_CirclePlane(DETECT_PARAM(circle, plane, collisionId))
{
	Vector2 planeNormal = plane->props.plane.normal;
	Vector2 planePos = plane->transform.position;
	Vector2 closestPointToPlane = circle->transform.position - planeNormal * circle->props.circle.radius;

	float dist = (planeNormal.x * (planePos.y - planeNormal.y) - planeNormal.y * (planePos.x - planeNormal.x));

	return CollisionWrapper
	(
		dist > 0,
		CollisionInfo
		(
			collisionId, circle, plane, planeNormal, dist, false // idk
		)
	);
}

const std::function<CollisionWrapper(DETECT_PARAM_TYPE)> detectFuncList[]
{
	&Detect_CircleCircle,
	&Detect_CirclePlane
};

const std::function<CollisionWrapper(DETECT_PARAM_TYPE)> GetDetectFunc(int i)
{
	return detectFuncList[i];
}



// Detect methods //////////////////////////////////

const void Solve_CircleCircle(CollisionInfo c)
{
	auto geom0 = c.a, geom1 = c.b;

	float massA = geom0->GetMass();
	float massB = geom1->GetMass();

	Vector2 velA = geom0->vel;
	Vector2 velB = geom1->vel;

	float minRestitution = std::min(geom0->restitution, geom1->restitution);
	float totalMass = massA + massB;

	geom0->vel = (velA * (massA - massB) + velB * massB * 2) / totalMass * minRestitution;
	geom1->vel = (velB * (massB - massA) + velA * massA * 2) / totalMass * minRestitution;
}

const void Solve_CirclePlane(CollisionInfo c)
{
	
}

const std::function<void(CollisionInfo)> solveFuncList[]
{
	&Solve_CircleCircle,
	&Solve_CirclePlane
};

const std::function<void(CollisionInfo)> GetSolveFunc(int i)
{
	return solveFuncList[i];
}