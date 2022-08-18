#pragma once

#include <iostream>
#include <math.h>
#include <vector>
#include <set>
#include <sstream>
#include <string>
#include "Entity.h"
#include "Geometry.h"


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
	vector<Geometry> geometries;
	Scene()
	{
		
	}

	void InitializeSceneTesting(int nOfTestEntities)
	{
		for (int i = 0; i < nOfTestEntities; i++)
		{
			float randRadius = (rand()) / float(RAND_MAX) / 10.0 + 0.025;

			Geometry g(
				Transform2D(
					Vector2((rand()) / float(RAND_MAX) / 2.0 - 0.25f, (rand()) / float(RAND_MAX) / 2.0 - 0.25f),
					0,
					Vector2(randRadius, randRadius),
					0
				),
				EntityType::Circle
			);
			g.color = Color(0, 0.5, 1);
			g.vel = Vector2(
				(rand()) / float(RAND_MAX) / 2.0 - 0.25f, 
				(rand()) / float(RAND_MAX) / 2.0 - 0.25f
			);
			geometries.push_back(g);
		}
	}

	void AddGeometry(Geometry g)
	{
		geometries.push_back(g);
	}

	Geometry* GetGeometryRef(int i)
	{
		return &geometries[i];
	}

	void RenderScene(int i, Shader* shader)
	{
		Geometry e = geometries[i];
		shader->SetUniformVec4("Color", e.color.r, e.color.g, e.color.b, e.color.a);
		shader->SetUniformVec2("Position", e.transform.position.x, e.transform.position.y);
		shader->SetUniformVec2("Size", e.transform.scale.x, e.transform.scale.y);
	}

	void NarrowCollisionsTest()
	{
		int size = geometries.size();
		vector<CollisionTestInfo> testList;

		for (int i = 0; i < size; i++)
		{
			geometries[i].color = Color(0, 0.5, 1);
			geometries[i].CalculateAABB();
		}
		for (int i = 0; i < size; i++)
		{
			for (int j = i + 1; j < size; j++)
			{
				bool cond = AABB::DetectCollisionTest(geometries[i].boundingBox, geometries[j].boundingBox);
				if (cond)
				{
					testList.push_back(CollisionTestInfo(i, j));
					geometries[i].color = Color(0, 1, 1);
					geometries[j].color = Color(0, 1, 1);
				}
			}
		}
		for (auto &t: testList)
		{
			Entity* a = &geometries[t.a];
			Entity* b = &geometries[t.b];
			if 
			(
				pow((a->transform.scale.x + b->transform.scale.x) / 2.0f, 2) >
				(a->transform.position - b->transform.position).lenSq()
			)
			{
				a->color = Color(0, 1, 0.5);
				b->color = Color(0, 1, 0.5);
			}
		}
	}

	void Update(float delta)
	{
		NarrowCollisionsTest();
		for (auto& t : geometries)
		{
			t.transform.position += t.vel * delta;
			Vector2 pos = t.transform.position;

			if (pos.x > 1)
			{
				t.transform.position.x -= 2;
			}
			else if (pos.x < -1)
			{
				t.transform.position.x += 2;
			}

			if (pos.y > 1)
			{
				t.transform.position.y -= 2;
			}
			else if (pos.y < -1)
			{
				t.transform.position.y += 2;
			}
		}
	}
};