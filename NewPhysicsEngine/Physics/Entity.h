#pragma once

#include "../Structs/Vector2.h"
#include "../Structs/Color.h"
#include "Transform2D.h"
#include "AABB.h"
#include "EntityDataStruct.h"

enum EntityType
{
	Nothing       = 0,

	// geometries
	Geometry      = 1,

	// constraint
	Constraint    = 2,

	// non-constraint
	Nonconstraint = 3
};

struct Entity
{
	// 1. Geometries, 2. Non-geometries, 3. External Components
	array<int, 3> entityRef;
	int id;

	EntityType entityType = EntityType::Nothing;

	// Appearance
	Color color;
	bool drawBorder = false;
	bool isGeometry = false;

	// Render bound
	Vector2 bound;

	// Entity-specific properties
	EntityData::EntityProp props;
	EntityData::Geom geom;

	Entity(EntityType type, int id, array<int, 3> entityRef)
	{
		this->entityType = type;
		this->isGeometry = (type >> 8) & 1;
		this->entityRef = entityRef;
		this->id = id;

		geom.SetArea();
		geom.SetMass();
	}

	Entity() {}
};
