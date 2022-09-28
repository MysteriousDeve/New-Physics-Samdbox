#pragma once

#include "../Structs/Vector2.h"
#include "../Structs/Color.h"
#include "Transform2D.h"
#include "AABB.h"
#include "EntityDataStruct.h"

enum EntityType
{
	Nothing    = 0,

	// geometries
	Circle     = 0x0100,
	Box        = 0x0101,
	Poly       = 0x0102,
	Plane      = 0x0103,

	// constraint
	Axle       = 0x0200,
	Spring     = 0x0201,
	Fixjoint   = 0x0202,

	// non-constraint
	Tracer     = 0x0303,
	Thruster   = 0x0304,
	Laser      = 0x0305,
};

struct Entity
{
	// 1. Geometries, 2. Non-geometries, 3. External Components
	int ref[3];

	EntityType entityType = EntityType::Nothing;

	// Appearance
	Color color;
	bool drawBorder = false;
	bool isGeometry = false;

	// Entity-specific properties
	EntityData::EntityProp props;
	EntityData::Geom geom;

	Entity(Transform2D transform, EntityType type)
	{
		this->transform = transform;
		this->entityType = type;
		this->isGeometry = (type >> 8) & 1;

		SetArea();
		SetMass();
	}

	Entity() {}

	void InitCircle()
	{

	}

	float CircleArea()
	{
		return pow(props.circle.radius, 2i32) * PI;
	}

	float BoxArea()
	{
		return transform.scale.InternalMultiply();
	}

	float PolygonArea()
	{
		return 1;
	}	
	
	float GetArea()
	{
		return geom.area;
	}

	float SetArea()
	{
		int id = entityType % 256;
		switch (id)
		{
			case 2:   return (geom.area = PolygonArea());
			case 1:   return (geom.area = BoxArea());
			default:  return (geom.area = CircleArea());
		}
	}

	float GetMass()
	{
		return geom.mass;
	}

	float SetMass()
	{
		if (isGeometry) return (geom.mass = geom.density * geom.area);
		throw exception("Set mass called on non-geometry objects");
	}
};

