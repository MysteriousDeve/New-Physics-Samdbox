#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <functional>

#include "../Renderer/Camera.h"
#include "../Renderer/Renderer.h"
#include "../Structs/Color.h"
#include "Button.h"

using namespace glm;

class InteractionHandler
{
private:
	GLFWwindow* window;
	Camera* camera;
	vector<Button*> buttons;
	Renderer* renderer;
public:
	InteractionHandler(GLFWwindow* window, Camera* camera)
	{
		this->window = window;
		this->camera = camera;

		GLfloat vert[]{
			 -0.5f, -0.5f, 0.0f,
			 -0.5f,  0.5f, 0.0f,
			  0.5f,  0.5f, 0.0f,
			  0.5f, -0.5f, 0.0f
		};
		GLuint index[]{
			0, 1, 2, 2, 3, 0
		};
		renderer = new Renderer(
			MeshInfo(vert, sizeof(vert), index, sizeof(index)),
			"Common", "Box"
		);
		renderer->Initialize();
	}

	~InteractionHandler()
	{
		renderer->Terminate();
		delete[] renderer;
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

	void UIRaycast()
	{
		Vector2 mousePos = GetMouseWorldPos();
		for (auto& b : buttons)
		{
			if (b->isInside(mousePos))
			{
				if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
				{
					b->OnClick();
				}
				else b->OnHover();
			}
			else b->OnIdle();
			b->Update();
		}
	}

	Button* AddButton(Vector2 position, Vector2 size)
	{
		Button* button = new Button(position, size, []() {}, []() {});
		buttons.push_back(button);
		return button;
	}

	void RenderButtons()
	{
		vector<Vector2> posVector;
		vector<Color> colorVector;
		for (auto& x : buttons)
		{
			posVector.push_back(x->transform.position);
			colorVector.push_back(x->color);
		}
		renderer->Render(
			buttons.size(), 
			[posVector, colorVector](int i, Shader* shader)
			{
				shader->SetColor("Color", colorVector[i]);
				shader->SetUniformVec2("Position", posVector[i].x, posVector[i].y);
			}
		);
	}
};