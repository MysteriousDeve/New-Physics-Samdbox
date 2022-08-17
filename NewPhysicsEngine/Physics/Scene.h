#pragma once

#include <math.h>
#include <vector>
#include <set>
#include <sstream>
#include "Entity.h"

using namespace std;

class Scene
{
private:
	struct CollisionTestInfo
	{
		int a, b;
		CollisionTestInfo(int a, int b)
		{
			this->a = a;
			this->b = b;
		}
	};
public:
	vector<Entity> entities;
	Scene()
	{
		
	}

	void InitializeSceneTesting(int nOfTestEntities)
	{
		for (int i = 0; i < nOfTestEntities; i++)
		{
			rand(); rand();
			Entity e(
				Transform2D(
					Vector2(rand() / float(RAND_MAX) - 0.5f, rand() / float(RAND_MAX) - 0.5f),
					0,
					Vector2(0.05f, 0.05f),
					0
				),
				EntityType::Circle
			);
			e.color = Color(0, 0.5, 1);
			entities.push_back(e);
		}
	}

	void AddEntity(Entity e)
	{
		entities.push_back(e);
	}

	void RenderScene(int i, Shader* shader)
	{
		Entity e = entities[i];
		shader->SetUniformVec4("Color", e.color.r, e.color.g, e.color.b, e.color.a);
		shader->SetUniformVec2("Position", e.transform.position.x, e.transform.position.y);
		shader->SetUniformVec2("Size", e.transform.scale.x, e.transform.scale.y);
	}

	void NarrowCollisionsTest()
	{
		int size = entities.size();
		vector<CollisionTestInfo> testList;

		for (int i = 0; i < size; i++)
		{
			entities[i].color = Color(0, 0.5, 1);
		}
		for (int i = 0; i < size; i++)
		{
			for (int j = i + 1; j < size; j++)
			{
				if (entities[i].boundingBox.DetectCollisionTest(entities[j].boundingBox))
				{
					testList.push_back(CollisionTestInfo(i, j));
					entities[i].color = Color(0, 1, 1);
					entities[j].color = Color(0, 1, 1);
				}
			}
		}
		for (auto &t: testList)
		{
			float r = rand() / float(RAND_MAX);
			float r2 = rand() / float(RAND_MAX);
			
			Entity* a = &entities[t.a];
			Entity* b = &entities[t.b];
			if 
			(
				pow((a->transform.scale.x + b->transform.scale.x) / 2.0f, 2) <
				(a->transform.position - b->transform.position).lenSq()
			)
			{
				a->color = Color(r, r2, 0);
				b->color = Color(r, r2, 0);
			}
		}
	}

	void Update(float delta)
	{

	}
};