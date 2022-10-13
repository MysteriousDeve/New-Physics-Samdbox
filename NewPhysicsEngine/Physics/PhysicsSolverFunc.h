#pragma once

#include "../ProjectInclude.h"

#include "Entity.h"

// Info wrapper

struct CollisionTestInfo
{
	int a, b;
	CollisionTestInfo(int a, int b)
	{
		this->a = a;
		this->b = b;
	}
};
struct CollisionInfo
{
	EntityData::Geom a, b;
	Vector2 normal;
	float depth;
	bool isMovingTowardEachOther;

	CollisionInfo(
		EntityData::Geom a, EntityData::Geom b, 
		Vector2 normal, 
		float depth, 
		bool isMovingTowardEachOther
	)
	{
		this->a = a;
		this->b = b;
		this->normal = normal;
		this->depth = depth;
		this->isMovingTowardEachOther = isMovingTowardEachOther;
	}

	CollisionInfo(){}
};
struct CollisionWrapper
{
	CollisionInfo info;
	int collisionType;
	bool isCollide;
	
	CollisionWrapper(int collisionType, bool isCollide, CollisionInfo info)
	{
		this->isCollide = isCollide;
		this->collisionType = collisionType;
		this->info = info;
	}

	operator bool()
	{
		return isCollide;
	}
};

int GetGeometryCollisionId(EntityType t0, EntityType t1)
{
	int n0 = t0 & 3, n1 = t1 & 3;
	return n0 * 4 + n1;
}

// Narrow-phase methods //////////////////////////////////
#define DETECT_PARAM(name0, name1) EntityData::Geom name0, EntityData::Geom name1
#define DETECT_PARAM_TYPE DETECT_PARAM(,)

const CollisionWrapper Detect_CircleCircle(DETECT_PARAM(c0, c1))
{
	Vector2 distVec = c0.transform.position - c1.transform.position;
	float dist = distVec.len() - c0.props.circle.radius + c1.props.circle.radius;
	return CollisionWrapper
	(
		dist > 0,
		CollisionInfo
		(
			collisionId, c0, c1, distVec.Normalize(), dist, false // idk
		)
	);
}
/* Not implemented */ const CollisionWrapper Detect_CircleBox(DETECT_PARAM(circle, box))
{
	return CollisionWrapper
	(
		false,
		CollisionInfo
		(
			circle, box, 0, 0, false
		)
	);
} 
/* Not implemented */ const CollisionWrapper Detect_CirclePolygon(DETECT_PARAM(circle, polygon))
{
	return CollisionWrapper
	(
		false,
		CollisionInfo
		(
			collisionId, c0, c1, 0, 0, false
		)
	);
}
const CollisionWrapper Detect_CirclePlane(DETECT_PARAM(circle, plane, collisionId))
{
	Vector2 planeNormal = Vector2::angleToNormal(plane.transform.rotation);
	Vector2 planePos = plane.transform.position;
	Vector2 closestPointToPlane = circle.transform.position - planeNormal * circle.props.circle.radius;

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

/* Not implemented */ const CollisionWrapper Detect_BoxBox(DETECT_PARAM(c0, c1))
{
	return CollisionWrapper
	(
		false,
		CollisionInfo
		(
			collisionId, c0, c1, 0, 0, false
		)
	);
}
/* Not implemented */ const CollisionWrapper Detect_BoxPolygon(DETECT_PARAM(box, poly))
{
	return CollisionWrapper
	(
		false,
		CollisionInfo
		(
			collisionId, c0, c1, 0, 0, false
		)
	);
}
/* Not implemented */ const CollisionWrapper Detect_BoxPlane(DETECT_PARAM(box, plane))
{
	return CollisionWrapper
	(
		false,
		CollisionInfo
		(
			collisionId, c0, c1, 0, 0, false
		)
	);
}

/* Not implemented */ const CollisionWrapper Detect_PolygonPolygon(DETECT_PARAM(c0, c1))
{
	return CollisionWrapper
	(
		false,
		CollisionInfo
		(
			collisionId, c0, c1, 0, 0, false
		)
	);
}
/* Not implemented */ const CollisionWrapper Detect_PolygonPlane(DETECT_PARAM(poly, plane))
{
	return CollisionWrapper
	(
		false,
		CollisionInfo
		(
			collisionId, c0, c1, 0, 0, false
		)
	);
}

const std::function<CollisionWrapper(DETECT_PARAM_TYPE)> detectFuncList[]
{
	// Circle group
	&Detect_CircleCircle,
	&Detect_CircleBox,
	&Detect_CirclePolygon,
	&Detect_CirclePlane,

	// Box group
	&Detect_CircleBox,
	&Detect_BoxBox,
	&Detect_BoxPolygon,
	&Detect_BoxPlane,

	// Polygon group
	&Detect_CirclePolygon,
	&Detect_BoxPolygon,
	&Detect_PolygonPolygon,
	&Detect_PolygonPlane,

	// Plane group (only need 3 since 2 planes do not collide)
	&Detect_CirclePlane,
	&Detect_BoxPlane,
	&Detect_PolygonPlane,
};

const std::function<CollisionWrapper(DETECT_PARAM_TYPE)> GetDetectFunc(int i)
{
	return detectFuncList[i];
}



// Solve methods //////////////////////////////////

const void Solve_CircleCircle(CollisionInfo c)
{
	auto geom0 = c.a, geom1 = c.b;

	float massA = geom0.GetMass();
	float massB = geom1.GetMass();

	Vector2 velA = geom0.vel;
	Vector2 velB = geom1.vel;

	float minRestitution = std::min(geom0.restitution, geom1.restitution);
	float totalMass = massA + massB;

	geom0.vel = (velA * (massA - massB) + velB * massB * 2) / totalMass * minRestitution;
	geom1.vel = (velB * (massB - massA) + velA * massA * 2) / totalMass * minRestitution;
}
/* Not implemented */ const void Solve_CircleBox(CollisionInfo c)
{
	return;
}
/* Not implemented */ const void Solve_CirclePolygon(CollisionInfo c)
{
	return;
}
const void Solve_CirclePlane(CollisionInfo c)
{
	
}

/* Not implemented */ const void Solve_BoxBox(CollisionInfo c)
{
	return;
}
/* Not implemented */ const void Solve_BoxPolygon(CollisionInfo c)
{
	return;
}
/* Not implemented */ const void Solve_BoxPlane(CollisionInfo c)
{
	return;
}

/* Not implemented */ const void Solve_PolygonPolygon(CollisionInfo c)
{
	return;
}
/* Not implemented */ const void Solve_PolygonPlane(CollisionInfo c)
{
	return;
}

const std::function<void(CollisionInfo)> solveFuncList[]
{
	// Circle group
	&Solve_CircleCircle,
	&Solve_CircleBox,
	&Solve_CirclePolygon,
	&Solve_CirclePlane,

	// Box group
	&Solve_CircleBox,
	&Solve_BoxBox,
	&Solve_BoxPolygon,
	&Solve_BoxPlane,

	// Polygon group
	&Solve_CirclePolygon,
	&Solve_BoxPolygon,
	&Solve_PolygonPolygon,
	&Solve_PolygonPlane,

	// Plane group (only need 3 since 2 planes do not collide)
	&Solve_CirclePlane,
	&Solve_BoxPlane,
	&Solve_PolygonPlane,
};

const std::function<void(CollisionInfo)> GetSolveFunc(int i)
{
	return solveFuncList[i];
}
const void Solve(CollisionInfo info)
{
	
}