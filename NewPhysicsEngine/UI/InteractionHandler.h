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

	Vector2 GetMouseScreenPos()
	{
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return Vector2(x, y);
	}

	Vector2 GetWindowSize()
	{
		int x, y;
		glfwGetWindowSize(window, &x, &y);
		return Vector2(x, y);
	}

	void UIRaycast()
	{
		Vector2 mousePos = GetMouseScreenPos();
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

		float WHRatio = camera->GetWHratio();
		Vector2 windowSize = GetWindowSize();

		vector<Vector2> posVector;
		vector<Vector2> sizeVector;
		vector<Color> colorVector;
		for (auto& x : buttons)
		{
			Vector2 pos = x->transform.position, size = x->transform.scale;
			Vector2 transformedSize (
				size.x / windowSize.x * 2,
				size.y / windowSize.y * 2
			);
			sizeVector.push_back(transformedSize);
			posVector.push_back(Vector2(
				(pos.x / windowSize.x - 0.5) * 2 + transformedSize.x / 2.0,
				- (pos.y / windowSize.y - 0.5) * 2 - transformedSize.y / 2.0
			));

			colorVector.push_back(x->color);
		}
		renderer->Render(
			buttons.size(), 
			[posVector, colorVector, sizeVector](int i, Shader* shader)
			{
				shader->SetColor("Color", colorVector[i]);
				shader->SetUniformVec2("Position", posVector[i].x, posVector[i].y);
				shader->SetUniformVec2("Size", sizeVector[i].x, sizeVector[i].y);
			}
		);
	}

	void OnScroll(GLFWwindow* window, double xoffset, double yoffset)
	{
		camera->Zoom(pow(1.5, yoffset));
	}
};