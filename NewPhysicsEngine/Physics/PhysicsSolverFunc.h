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

int GetGeometryCollisionId(GeometryType t0, GeometryType t1)
{
	return t0 * 4 + t1;
}
constexpr int GetGeometryCollisionIdConst(GeometryType t0, GeometryType t1)
{
	return t0 * 4 + t1;
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
		0,
		dist > 0,
		CollisionInfo
		(
			c0, c1, distVec.Normalize(), dist, false // idk
		)
	);
}
/* Not implemented */ const CollisionWrapper Detect_CircleBox(DETECT_PARAM(circle, box))
{
	return CollisionWrapper
	(
		GetGeometryCollisionIdConst(GeometryType::Circle, GeometryType::Box),
		false,
		CollisionInfo
		(
			circle, box, 0, 0, false
		)
	);
} 
/* Not implemented */ const CollisionWrapper Detect_CirclePolygon(DETECT_PARAM(circle, poly))
{
	return CollisionWrapper
	(
		GetGeometryCollisionIdConst(GeometryType::Circle, GeometryType::Poly),
		false,
		CollisionInfo
		(
			circle, poly, 0, 0, false
		)
	);
}
const CollisionWrapper Detect_CirclePlane(DETECT_PARAM(circle, plane))
{
	Vector2 planeNormal = Vector2::angleToNormal(plane.transform.rotation);
	Vector2 planePos = plane.transform.position;
	Vector2 closestPointToPlane = circle.transform.position - planeNormal * circle.props.circle.radius;

	float dist = (planeNormal.x * (planePos.y - planeNormal.y) - planeNormal.y * (planePos.x - planeNormal.x));

	return CollisionWrapper
	(
		GetGeometryCollisionIdConst(GeometryType::Circle, GeometryType::Plane),
		dist > 0,
		CollisionInfo
		(
			circle, plane, planeNormal, dist, false // idk
		)
	);
}

/* Not implemented */ const CollisionWrapper Detect_BoxBox(DETECT_PARAM(c0, c1))
{
	return CollisionWrapper
	(
		GetGeometryCollisionIdConst(GeometryType::Box, GeometryType::Box),
		false,
		CollisionInfo
		(
			c0, c1, 0, 0, false
		)
	);
}
/* Not implemented */ const CollisionWrapper Detect_BoxPolygon(DETECT_PARAM(box, poly))
{
	return CollisionWrapper
	(
		GetGeometryCollisionIdConst(GeometryType::Box, GeometryType::Poly),
		false,
		CollisionInfo
		(
			box, poly, 0, 0, false
		)
	);
}
/* Not implemented */ const CollisionWrapper Detect_BoxPlane(DETECT_PARAM(box, plane))
{
	return CollisionWrapper
	(
		GetGeometryCollisionIdConst(GeometryType::Box, GeometryType::Plane),
		false,
		CollisionInfo
		(
			box, plane, 0, 0, false
		)
	);
}

/* Not implemented */ const CollisionWrapper Detect_PolygonPolygon(DETECT_PARAM(c0, c1))
{
	return CollisionWrapper
	(
		GetGeometryCollisionIdConst(GeometryType::Poly, GeometryType::Poly),
		false,
		CollisionInfo
		(
			c0, c1, 0, 0, false
		)
	);
}
/* Not implemented */ const CollisionWrapper Detect_PolygonPlane(DETECT_PARAM(poly, plane))
{
	return CollisionWrapper
	(
		GetGeometryCollisionIdConst(GeometryType::Poly, GeometryType::Plane),
		false,
		CollisionInfo
		(
			poly, plane, 0, 0, false
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

const void Detect(EntityData::Geom g0, EntityData::Geom g1)
{
	int type = GetGeometryCollisionId(g0.type, g1.type);
	GetDetectFunc(type)(g0, g1);
}


// Solve methods //////////////////////////////////
#define SOLVE_RETURN void
#define SOLVE_PARAM CollisionInfo c, p_dec dt
#define SOLVE_FUNC_DEF SOLVE_RETURN(SOLVE_PARAM)

const SOLVE_RETURN Solve_CircleCircle(SOLVE_PARAM)
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
/* Not implemented */ const SOLVE_RETURN Solve_CircleBox(SOLVE_PARAM)
{
	return;
}
/* Not implemented */ const SOLVE_RETURN Solve_CirclePolygon(SOLVE_PARAM)
{
	return;
}
const SOLVE_RETURN Solve_CirclePlane(SOLVE_PARAM)
{
	return;
}

/* Not implemented */ const SOLVE_RETURN Solve_BoxBox(SOLVE_PARAM)
{
	return;
}
/* Not implemented */ const SOLVE_RETURN Solve_BoxPolygon(SOLVE_PARAM)
{
	return;
}
/* Not implemented */ const SOLVE_RETURN Solve_BoxPlane(SOLVE_PARAM)
{
	return;
}

/* Not implemented */ const SOLVE_RETURN Solve_PolygonPolygon(SOLVE_PARAM)
{
	return;
}
/* Not implemented */ const SOLVE_RETURN Solve_PolygonPlane(SOLVE_PARAM)
{
	return;
}

const std::function<SOLVE_FUNC_DEF> solveFuncList[]
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

const std::function<SOLVE_FUNC_DEF> GetSolveFunc(int i)
{
	return solveFuncList[i];
}

const SOLVE_RETURN Solve(CollisionWrapper col, p_dec dt)
{
	if (col)
	{
		GetSolveFunc(col.collisionType)(col.info, dt);
	}
}