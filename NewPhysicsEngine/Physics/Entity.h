#pragma once

#include "../Structs/Vector2.h"
#include "../Structs/Color.h"
#include "Transform2D.h"
#include "AABB.h"

enum class EntityType
{
	Nothing,
	Circle
};

class Entity
{
public:
	Transform2D transform;
	EntityType entityType = EntityType::Nothing;

	// Appearance
	Color color;

	Entity(Transform2D transform, EntityType type)
	{
		this->transform = transform;
		this->entityType = type;
	}
};