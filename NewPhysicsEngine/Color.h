#pragma once

struct Color
{
	float r = 0, g = 0, b = 0, a = 0;

	Color(float r, float g, float b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		a = 1;
	}

	Color(float r, float g, float b, float a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	Color()
	{

	}
};