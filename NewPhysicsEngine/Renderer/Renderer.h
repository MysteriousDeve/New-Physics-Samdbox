#pragma once

#include <functional>

#include "Mesh.h"
#include "Shader.h"
#include "../Physics/Scene.h"

class Renderer
{
private:
	Mesh* mesh = nullptr;
	Shader* shader = nullptr;

public:
	Renderer(MeshInfo meshInfo, string shaderName)
	{
		mesh = new Mesh(meshInfo);
		shader = new Shader(shaderName);
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
};