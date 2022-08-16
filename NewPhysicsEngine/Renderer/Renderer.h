#pragma once

#include <functional>
#include <glm/glm.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "../Physics/Scene.h"

class Renderer
{
private:
	Mesh* mesh = nullptr;
	Shader* shader = nullptr;

public:
	Renderer(MeshInfo meshInfo, string vert, string frag)
	{
		mesh = new Mesh(meshInfo);
		shader = new Shader(vert, frag);
	}
	
	void Initialize()
	{
		shader->Initialize();
		mesh->Initialize();
	}

	void Render()
	{
		shader->Use();
		mesh->Bind();
		mesh->Draw();
	}

	void Render(int times, std::function<void(int, Shader*)> onDraw)
	{
		shader->Use();
		mesh->Bind();
		for (int i = 0; i < times; i++)
		{
			onDraw(i, shader);
			mesh->Draw();
		}
	}

	void Terminate()
	{
		mesh->Terminate();
		shader->DestroyShaderProgram();
	}

	void SetCameraTransformation(glm::mat4x4 mat, glm::vec2 pan)
	{
		shader->SetUniformMat4x4("CameraMatrix", mat);
		shader->SetUniformVec2("Pan", pan[0], pan[1]);
	}
};