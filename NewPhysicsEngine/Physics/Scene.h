#pragma once

#include <math.h>
#include <vector>
#include "Entity.h"

using namespace std;

class Scene
{
private:
	
public:
	vector<Entity> entities;
	Scene()
	{
		
	}

	void InitializeSceneTesting(int nOfTestEntities)
	{
		for (int i = 0; i < nOfTestEntities; i++)
		{
			Entity e(
				Transform2D(
					Vector2(rand() / float(RAND_MAX) - 0.5, rand() / float(RAND_MAX) - 0.5),
					0,
					Vector2(0.1, 0.1),
					0
				),
				EntityType::Circle
			);
			e.color = Color(0, 0.5, 1);
			entities.push_back(e);
		}
	}

	void RenderScene(int i, Shader* shader)
	{
		Entity e = entities[i];
		shader->SetUniformVec4("Color", e.color.r, e.color.g, e.color.b, e.color.a);
		shader->SetUniformVec2("Position", e.transform.position.x, e.transform.position.y);
		shader->SetUniformFloat("Radius", e.transform.scale.Max());
	}
};