#pragma once

#include "../Structs/Vector2.h"

struct PrimitiveTransform2D
{
	// The center of mass
	Vector2 position = Vector2(0, 0);

	PrimitiveTransform2D()
	{

	}

	PrimitiveTransform2D(Vector2 position)
	{
		this->position = position;
	}
};


struct Transform2D : PrimitiveTransform2D
{
	float rotation = 0;

	Transform2D()
	{

	}

	Transform2D(Vector2 position, float rotation, Vector2 scale)
	{
		this->position = position;
		this->rotation = rotation;
	}
};

struct Transform2DUI : PrimitiveTransform2D
{
	Vector2 scale = Vector2(1, 1);
	float depth = 0;

	Transform2DUI()
	{

	}

	Transform2DUI(Vector2 position, Vector2 scale, float depth)
	{
		this->position = position;
		this->scale = scale;
		this->depth = depth;
	}
};
