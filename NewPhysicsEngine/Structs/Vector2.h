#pragma once

#include <math.h>
#include <glm/glm.hpp>
using namespace std;

struct Vector2
{
	float x = 0, y = 0;

	Vector2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	Vector2(float xy)
	{
		this->x = xy;
		this->y = xy;
	}

	
	Vector2()
	{

	}


	//--------------------------------------------------
	// Operators ---------------------------------------
	//--------------------------------------------------

	Vector2 operator+(Vector2 a)
	{
		return Vector2(x + a.x, y + a.y);
	}

	Vector2 operator-(Vector2 a)
	{
		return Vector2(x - a.x, y - a.y);
	}

	Vector2 operator*(float multiplier)
	{
		return Vector2(x * multiplier, y * multiplier);
	}

	Vector2 operator/(float multiplier)
	{
		return Vector2(x / multiplier, y / multiplier);
	}

	Vector2 operator+=(Vector2 a)
	{
		return Vector2(x += a.x, y += a.y);
	}

	Vector2 operator-=(Vector2 a)
	{
		return Vector2(x -= a.x, y -= a.y);
	}

	bool operator==(Vector2 a)
	{
		return !(x != a.x || y != a.y);
	}

	bool operator!=(Vector2 a)
	{
		return !(x == a.x || y == a.y);
	}

	//--------------------------------------------------
	// Functions ---------------------------------------
	//--------------------------------------------------

	// The length of the vector
	float len()
	{
		return sqrt(pow(x, 2) + pow(x, 2));
	}

	float lenSq()
	{
		return x * x + y * y;
	}

	Vector2 Normalize()
	{
		float l = len();
		return( 
			*this = l != 0 ? 
				*this / l : 
				Vector2(1, 0)
		);
	}

	// The angle of the vector in radian
	float GetAngle()
	{
		return atan2(y, x);
	}

	// Max value of the elements in the vector
	float Max()
	{
		return x > y ? x : y;
	}

	// Min value of the elements in the vector
	float Min()
	{
		return x < y ? x : y;
	}

	// Rotate the vector by a specified angle in radian
	Vector2 Rotate(float angle)
	{
		float c = cos(angle);
		float s = sin(angle);
		return (*this = Vector2(c * x - s * y, s * x + c * y));
	}

	//--------------------------------------------------
	// Casts -------------------------------------------
	//--------------------------------------------------

	operator glm::vec2() const
	{
		return glm::vec2(x, y);
	}
};