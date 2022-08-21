#pragma once

#include <glm/glm.hpp>
#include "../Structs/Vector2.h"

#define MATRIX_IDENTITY glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1)

using namespace glm;

class Camera
{
private:
	Vector2 pan;
	float zoom;
	float rotation;
	float WHratio;

	vec2 lastPan;
	bool panStarted = false;
public:
	Camera(Vector2 pan, float zoom, float rotation, float WHratio)
	{
		SetCameraState(pan, zoom, rotation, WHratio);
	}

	void SetCameraState(Vector2 pan, float zoom, float rotation, float WHratio)
	{
		this->pan = pan;
		this->zoom = zoom;
		this->rotation = rotation;
		this->WHratio = WHratio;
	}

	void SetWHratio(float WHratio)
	{
		this->WHratio = WHratio;
	}
	float GetWHratio()
	{
		return WHratio;
	}

	void SetRotation(float rotation)
	{
		this->rotation = rotation;
	}

	void Translate(Vector2 amount)
	{
		pan += amount;
	}

	mat4x4 GetCameraMatrix()
	{
		float c = cos(rotation);
		float s = sin(rotation);

		return glm::inverse(
			GetRotationMatrix() *
			GetScalingMatrix()
		);
	}
	mat4x4 GetRotationMatrix()
	{
		float c = cos(rotation);
		float s = sin(rotation);

		return mat4x4(
			c, s, 0, 0,
			-s, c, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	}
	mat4x4 GetScalingMatrix()
	{
		return mat4x4(
			zoom * WHratio, 0,    0, 0,
			0,              zoom, 0, 0,
			0,              0,    1, 0,
			0,              0,    0, 1
		);
	}

	vec2 GetPosition()
	{
		return vec2(
			pan.x, pan.y
		);
	}
	
	vec2 dragPan(vec2 mousePos)
	{
		return vec2();
	}
};