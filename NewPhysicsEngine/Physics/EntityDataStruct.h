#pragma once

#include "../ProjectInclude.h"

constexpr unsigned int WATER_COLLISION = 1 << 30;
constexpr unsigned int HETERO_COLLISION = 1 << 31;

enum VisualizationEnum
{
	FORCE                = 1u,
	MOMENTUM             = 2u,
	VELOCITY             = 4u,
	ANGULAR_VELOCITY     = 8u,
};

namespace EntityData
{
	struct Geom
	{
		/*
			Collision filter properties.
			The first 30 bits is for collision layer,
			the 31th is for water collision,
			and 32th is for hetero-collision.
		 */
		unsigned int collideSet = 0x01 | WATER_COLLISION;

		Vector2 COM;
		Vector2 angle;

		float gravityScale = 1;

		Vector2 vel;
		float angvel;

		float density;

		float area = 1;
		float mass = 1;

		float friction;
		float restitution;

		float reflectiveness;
		float refractiveIndex;
		float adhesion;
		
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
		int geom0, geom1;
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
	struct Nonconstraint
	{
		Vector2 relativePosition;
	};
	struct Tracer
	{
		float fadeTime;
		const bool independent = false;
	};
	struct Thruster
	{
		float force;
		float rotation;
		const bool independent = false;
	};
	struct Laser
	{
		float fadeDist;
		float laserVelocity;
		float rotation;
		bool independent;
		bool followGeometry = true;
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
