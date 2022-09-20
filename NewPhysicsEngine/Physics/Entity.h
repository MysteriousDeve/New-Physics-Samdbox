#pragma once

#include "../Structs/Vector2.h"
#include "../Structs/Color.h"
#include "Transform2D.h"
#include "AABB.h"
#include "EntityUnion.h"

constexpr unsigned int WATER_COLLISION  = 1 << 30;
constexpr unsigned int HETERO_COLLISION = 1 << 31;

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
	Transform2D transform;
	EntityType entityType = EntityType::Nothing;

	// Appearance
	Color color;
	bool drawBorder = false;
	bool isGeometry = false;

	// Entity-specific properties
	EntityUnion::EntityProp props;

	/*
		Collision filter properties.
		The first 30 bits is for collision layer,
			the 31th is for water collision,
			and 32th is for hetero-collision.
	 */
	unsigned int collideSet = 0x01 | WATER_COLLISION;

	Entity(Transform2D transform, EntityType type)
	{
		this->transform = transform;
		this->entityType = type;
		this->isGeometry = (type >> 8) & 1;
	}
};