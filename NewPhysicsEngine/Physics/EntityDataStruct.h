#pragma once

#include "../ProjectInclude.h"

constexpr p_int WATER_COLLISION = 1 << 30;
constexpr p_int HETERO_COLLISION = 1 << 31;

enum VisualizationEnum
{
	FORCE                = 1u,
	MOMENTUM             = 2u,
	VELOCITY             = 4u,
	ANGULAR_VELOCITY     = 8u,
};

enum GeometryType
{
	Circle,
	Box,
	Poly,
	Plane
};

enum ConstraintType
{
	Axle,
	Spring,
	Fixjoint
};

enum NonconstraintType
{
	Tracer,
	Thruster,
	Laser
};

namespace EntityData
{
	struct Circle
	{
		p_int radius;
		bool drawAngleIndicator;
	};
	struct Box
	{
		Vector2 size;
	};
	struct Poly
	{
		std::vector<std::vector<Vector2>*>* processedSurfaces;
		std::vector<Vector2>* rawSurfaces;
	};
	struct Plane
	{

	};
	union GeometryProps
	{
		Circle circle;
		Box box;
		Poly polygon;
		Plane plane;

		GeometryProps(){}
	};

	struct Geom
	{
		bool isExist = true;
		operator bool() const
		{
			return isExist;
		}

		GeometryType type;

		Transform2D transform;
		AABB aabb;

		/*
			Collision filter properties.
			The first 30 bits is for collision layer,
			the 31th is for water collision,
			and 32th is for hetero-collision.
		 */
		p_int collideSet = 0x01 | WATER_COLLISION;

		p_dec gravityScale = 1;

		Vector2 vel;
		p_dec angvel;

		p_dec density;

		p_dec area = 1;
		p_dec mass = 1;

		p_dec friction;
		p_dec restitution;

		p_dec reflectiveness;
		p_dec refractiveIndex;
		p_dec adhesion;
		
		p_int visualizationMode = NULL;

		GeometryProps props;

		p_dec CircleArea()
		{
			return pow(props.circle.radius, 2i32) * PI;
		}
		p_dec BoxArea()
		{
			return props.box.size.InternalMultiply();
		}
		p_dec PolygonArea()
		{
			return 1;
		}

		p_dec SetArea()
		{
			switch (type)
			{
			case GeometryType::Poly:    return area = PolygonArea();
			case GeometryType::Box:     return area = BoxArea();
			case GeometryType::Circle:  return area = CircleArea();
			default: return area = INFINITY;
			}
		}
		p_dec GetArea()
		{
			return area;
		}

		p_dec GetMass()
		{
			return mass;
		}
		p_dec SetMass()
		{
			return (mass = density * area);
		}

		void CalculateAABB()
		{
			
		}
	};

	// Constraint
	struct Constraint
	{
		p_int geom0, geom1;
		Vector2 geom0pos, geom1pos;
	};
	struct Axle : Constraint
	{
		// Motor
		bool motorEnabled;
		p_dec motorSpeed; // radian per second
		p_dec motorTorque;
	
		// Bend
		bool bendEnabled;
		p_dec bendTarget;
		p_dec bendConstant;
	};
	struct Spring : Constraint
	{
		p_dec springConstant;
		p_dec targetLength;
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
		p_dec fadeTime;
		const bool independent = false;
	};
	struct Thruster
	{
		p_dec force;
		p_dec rotation;
		const bool independent = false;
	};
	struct Laser
	{
		p_dec fadeDist;
		p_dec laserVelocity;
		p_dec rotation;
		bool independent;
		bool followGeometry = true;
	};

	// Union to save data and improve performance
	union EntityProp
	{
		Axle axle;
		Spring spring;
		Fixjoint fixjoint;

		Tracer tracer;
		Thruster thruster;
		Laser laser;

		EntityProp(){}
	};
}
