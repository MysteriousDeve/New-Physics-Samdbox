#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "../Renderer/Camera.h"

using namespace glm;

class InteractionHandler
{
private:
	GLFWwindow* window;
	Camera* camera;
public:
	InteractionHandler(GLFWwindow* window, Camera* camera)
	{
		this->window = window;
		this->camera = camera;
	}

	Vector2 GetMouseWorldPos()
	{
		double x, y;
		int w, h;
		glfwGetCursorPos(window, &x, &y);
		glfwGetWindowSize(window, &w, &h);
		vec2 untransformedMousePos = vec2(
			(float(x) / float(w) * 2 - 1),
			-float(y) / float(h) * 2 + 1
		);
		vec2 vec = glm::vec2(camera->GetCameraMatrix() * vec4(untransformedMousePos, 0, 0));
		return Vector2(vec[0] * pow(camera->GetWHratio(), 2), vec[1]);
	}
};