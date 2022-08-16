#pragma once

#include "../Structs/Vector2.h"

// Struct for Axis-Aligned Bounding Box
struct AABB
{
	float x = 0, y = 0, 
		  w = 0, h = 0;

	AABB()
	{

	}

	AABB(Vector2 position, float radius)
	{
		x = position.x;
		y = position.y;
		w = radius * 2;
		h = w;
	}

	bool DetectCollisionTest(AABB other)
	{
		return
			x < other.x + other.w &&
			x + w > other.x &&
			y < other.y + other.h &&
			h + y > other.y;
	}
};