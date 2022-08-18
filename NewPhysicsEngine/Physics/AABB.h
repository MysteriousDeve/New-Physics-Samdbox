#pragma once

#include "../Structs/Vector2.h"

// Struct for Axis-Aligned Bounding Box
struct AABB
{
	float w = 0, h = 0;
	Vector2 pos;

	AABB()
	{

	}

	AABB(Vector2 position, float squareSize)
	{
		this->pos = position - Vector2(squareSize / 2.0);
		w = squareSize;
		h = squareSize;
	}

	static bool DetectCollisionTest(AABB a, AABB b)
	{
		return
			a.pos.x < b.pos.x + b.w &&
			a.pos.x + a.w > b.pos.x &&
			a.pos.y < b.pos.y + b.h &&
			a.h + a.pos.y > b.pos.y;
	}
};