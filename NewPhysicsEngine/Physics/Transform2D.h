#pragma once

#include "../Structs/Vector2.h"

struct Transform2D
{
	// The center of mass
	Vector2 position = Vector2(0, 0);
	Vector2 scale = Vector2(1, 1);
	float depth = 0;

	Transform2D()
	{

	}

	Transform2D(Vector2 position, Vector2 scale, float depth)
	{
		this->position = position;
		this->scale = scale;
		this->depth = depth;
	}
};
