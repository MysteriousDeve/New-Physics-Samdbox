#pragma once

#include <glm/glm.hpp>
#include "../Structs/Vector2.h"

using namespace glm;

class Camera
{
private:
	Vector2 pan;
	float zoom;
	float rotation;
	float WHratio;
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

	mat4x4 GetCameraMatrix()
	{
		float c = cos(rotation);
		float s = sin(rotation);

		return glm::inverse(mat4x4(
			zoom * WHratio, 0,    pan.x, 0,
			0,              zoom, pan.y, 0,
			0,              0,    1,     0,
			0,              0,    0,     1
		) * mat4x4(
			 c, s, 0, 0,
			-s, c, 0, 0,
			 0, 0, 1, 0,
			 0, 0, 0, 1
		));
	}
};