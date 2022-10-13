#pragma once

#include <math.h>
#include <glm/glm.hpp>
using namespace std;

struct Vector2
{
	p_dec x = 0, y = 0;
	union { p_dec xy[1]; } xx, yy;

	Vector2(p_dec x, p_dec y)
	{
		this->x = x;
		this->y = y;
	}

	Vector2(p_dec xy)
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

	Vector2 operator*(p_dec multiplier)
	{
		return Vector2(x * multiplier, y * multiplier);
	}

	Vector2 operator/(p_dec multiplier)
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

	Vector2 operator*=(p_dec multiplier)
	{
		return Vector2(x *= multiplier, y *= multiplier);
	}

	Vector2 operator/=(p_dec multiplier)
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
	p_dec len()
	{
		return sqrt(lenSq());
	}

	p_dec lenSq()
	{
		return x * x + y * y;
	}

	Vector2 Normalize()
	{
		p_dec l = len();
		return( 
			*this = (l != 0 ? 
				*this / l : 
				Vector2(1, 0))
		);
	}

	// The angle of the vector in radian
	p_dec GetAngle()
	{
		return atan2(y, x);
	}

	// Max value of the elements in the vector
	p_dec Max()
	{
		return std::max(x, y);
	}

	// Min value of the elements in the vector
	p_dec Min()
	{
		return std::min(x, y);
	}

	// Rotate the vector by a specified angle in radian
	Vector2 Rotate(p_dec angle)
	{
		p_dec c = cos(angle);
		p_dec s = sin(angle);
		return (*this = Vector2(c * x - s * y, s * x + c * y));
	}

	p_dec InternalMultiply()
	{
		return x * y;
	}
	

	operator glm::vec2() const
	{
		return glm::vec2(x, y);
	};
	
	static p_dec Dot(Vector2 v1, Vector2 v2)
	{
		return v1.x * v2.x + v1.y * v2.y;
	}

	static Vector2 angleToNormal(p_dec angle)
	{
		return Vector2(cos(angle), sin(angle));
	}
};