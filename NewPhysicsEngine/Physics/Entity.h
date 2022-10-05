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
	array<int, 3> ref;
	int id;

	EntityType entityType = EntityType::Nothing;

	// Appearance
	Color color;
	bool drawBorder = false;
	bool isGeometry = false;

	// Entity-specific properties
	EntityData::EntityProp props;
	EntityData::Geom geom;

	Entity(EntityType type, int id, array<int, 3> ref)
	{
		this->entityType = type;
		this->isGeometry = (type >> 8) & 1;
		this->ref = ref;
		this->id = id;

		SetArea();
		SetMass();
	}

	Entity() {}
};

