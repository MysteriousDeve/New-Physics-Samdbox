#pragma once

#include <math.h>
#include <glm/glm.hpp>
using namespace std;

struct Vector2
{
	float x = 0, y = 0;
	union { float xy[1]; } xx, yy;

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

	Vector2 operator*=(float multiplier)
	{
		return Vector2(x *= multiplier, y *= multiplier);
	}

	Vector2 operator/=(float multiplier)
	{
		return Vector2(x /= multiplier, y /= multiplier);
	}

	Vector2 operator+()
	{
		return *this;
	}

	Vector2 operator-()
	{
		return Vector2(-x, -y);
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

	void Clear()
	{
		*this = Vector2();
	}

	// The length of the vector
	float len()
	{
		return sqrt(lenSq());
	}

	float lenSq()
	{
		return x * x + y * y;
	}

	Vector2 Normalize()
	{
		float l = len();
		return( 
			*this = (l != 0 ? 
				*this / l : 
				Vector2(1, 0))
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
		return std::max(x, y);
	}

	// Min value of the elements in the vector
	float Min()
	{
		return std::min(x, y);
	}

	// Rotate the vector by a specified angle in radian
	Vector2 Rotate(float angle)
	{
		float c = cos(angle);
		float s = sin(angle);
		return (*this = Vector2(c * x - s * y, s * x + c * y));
	}

	float InternalMultiply()
	{
		return x * y;
	}
	

	operator glm::vec2() const
	{
		return glm::vec2(x, y);
	};
	
	static float Dot(Vector2 v1, Vector2 v2)
	{
		return v1.x * v2.x + v1.y * v2.y;
	}

	static Vector2 angleToNormal(float angle)
	{
		return Vector2(cos(angle), sin(angle));
	}
};