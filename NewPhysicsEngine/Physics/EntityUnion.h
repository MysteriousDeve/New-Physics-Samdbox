#pragma once

#include "../ProjectInclude.h"

enum VisualizationEnum
{
	FORCE                = 1u,
	MOMENTUM             = 2u,
	VELOCITY             = 4u,
	ANGULAR_VELOCITY     = 8u,
};

namespace EntityData
{
	// Geometries
	struct Geom
	{
		Vector2 COM; // Center of mass
		Vector2 angle;

		float adhesion;
		float density;
		float friction;
		float restitution;
		float reflectiveness;
		float refractiveIndex;
		float gravityScale = 1;
		float area = 1;
		unsigned char visualizationMode = NULL;
	};
	struct Circle
	{
		float radius;
		bool drawAngleIndicator;
	};
	struct Box
	{

	};
	struct Poly
	{
		std::vector<std::vector<Vector2>*>* processedSurfaces;
		std::vector<Vector2>* rawSurfaces;
	};
	struct Plane
	{
		Vector2 normal;
	};

	// Constraint
	struct Constraint
	{
		float geom0, geom1;
		Vector2 geom0pos, geom1pos;
	};
	struct Axle : Constraint
	{
		// Motor
		bool motorEnabled;
		float motorSpeed; // radian per second
		float motorTorque;
	
		// Bend
		bool bendEnabled;
		float bendTarget;
		float bendConstant;
	};
	struct Spring : Constraint
	{
		float springConstant;
		float targetLength;
	};
	struct Fixjoint : Constraint
	{
		
	};

	// Non-constraint
	struct Tracer
	{
		float fadeTime;
	};
	struct Thruster
	{
		float force;
	};
	struct Laser
	{
		float fadeDist;
		float laserVelocity;
	};

	// Union to save data and improve performance
	union EntityProp
	{
		Circle circle;
		Box box;
		Poly polygon;
		Plane plane;

		Axle axle;
		Spring spring;
		Fixjoint fixjoint;

		Tracer tracer;
		Thruster thruster;
		Laser laser;

		EntityProp()
		{

		}
	};
}
