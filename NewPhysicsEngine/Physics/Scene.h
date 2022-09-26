#pragma once

#include <iostream>
#include <math.h>
#include <vector>
#include <set>
#include <sstream>
#include <string>
#include "Entity.h"
#include "Geometry.h"

#include "PhysicsSolverFunc.h"

using namespace std;

class Scene
{
public:
	vector<Entity> entities;
	vector<EntityData::Geom> geometries;

	Vector2 gravity = Vector2(0, -10);

	// Constructor
	Scene() {}

	void InitializeSceneTesting(int nOfTestEntities);

	void AddGeometry(Geometry g)
	{
		geometries.push_back(g);
	}
	Geometry* GetGeometryRef(int i)
	{
		return &geometries[i];
	}

	// Apply gravity
	void ApplyGravity(float dt)
	{
		for (auto& x : geometries)
		{
			x.vel += gravity * dt * (x.isGeometry && (x.entityType & 1) < 3) * x.geom.gravityScale;
		}
	}

	// Collisions detection
	vector<CollisionTestInfo> SpatialSubdivisionTest();
	vector<CollisionTestInfo> AABBCollisionsTest(vector<Geometry> geometries)
	{
		int size = geometries.size();
		vector<CollisionTestInfo> testList;
		for (int i = 0; i < size; i++)
		{
			for (int j = i + 1; j < size; j++)
			{
				bool cond = AABBPairTest(geometries[i], geometries[j]);
				if (cond)
				{
					testList.push_back(CollisionTestInfo(i, j));
				}
			}
		}
		return testList;
	}
	bool AABBPairTest(Geometry a, Geometry b)
	{
		bool cond = AABB::DetectCollisionTest(a.boundingBox, b.boundingBox);
		if (cond)
		{
			a.color = Color(0, 0.75, 1);
			b.color = Color(0, 0.75, 1);
		}
		return cond;
	}

	vector<CollisionInfo> NarrowCollisionsTest(vector<CollisionTestInfo> testList)
	{
		int size = geometries.size();
		vector<CollisionInfo> outputList;

		for (auto &t: testList)
		{
			Geometry* a = &geometries[t.a];
			Geometry* b = &geometries[t.b];
			float depth = pow((a->transform.scale.x + b->transform.scale.x) / 2.0f, 2) - (a->transform.position - b->transform.position).lenSq();
			if (depth > 0)
			{
				a->color = Color(0, 1, 1);
				b->color = Color(0, 1, 1);

				Vector2 VectorAB = b->transform.position - a->transform.position;

				outputList.push_back(
					CollisionInfo
					(
						t.a, t.b,
						VectorAB.Normalize(),
						depth,
						Vector2::Dot(a->vel, VectorAB) > 0 && Vector2::Dot(b->vel, -VectorAB) > 0
					)
				);
			}
		}
		return outputList;
	}

	vector<CollisionInfo> DetectCollision()
	{
		for (int i = 0; i < geometries.size(); i++)
		{
			geometries[i].color = Color(0, 0.5, 1);
			geometries[i].CalculateAABB();
		}

		return NarrowCollisionsTest(AABBCollisionsTest(geometries));
		//NarrowCollisionsTest(
		//	SpatialSubdivisionTest()
		//);
		
	}
	
	// Collisions solver
	void SolveCollision(vector<CollisionInfo> collisionList, float dt)
	{
		for (auto& t : collisionList)
		{
			if (t.isMovingTowardEachOther)
			{
				Geometry geomA = geometries[t.a];
				Geometry geomB = geometries[t.b];

				float massA = geomA.GetMass();
				float massB = geomB.GetMass();

				Vector2 velA = geomA.GetVelocity();
				Vector2 velB = geomB.GetVelocity();

				float minRestitution = std::min(geomA.restitution, geomB.restitution);
				float totalMass = massA + massB;

				geometries[t.a].vel = (velA * (massA - massB) + velB * massB * 2) / totalMass * minRestitution;
				geometries[t.b].vel = (velB * (massB - massA) + velA * massA * 2) / totalMass * minRestitution;
			}
		}
	}
	void SolveOverlap(vector<CollisionInfo> collisionList, float dt)
	{
		for (auto& t : collisionList)
		{
			float densitySum = geometries[t.a].density + geometries[t.b].density;
			Vector2 contactForce = t.normal * t.depth;

			float massA = geometries[t.a].GetMass(), massB = geometries[t.b].GetMass();

			Vector2 massSolveA = contactForce * massA / densitySum * 2,
					massSolveB = contactForce * massB / densitySum * 2;

			geometries[t.a].transform.position -= massSolveA;
			geometries[t.b].transform.position += massSolveB;

			geometries[t.a].vel -= massSolveA;
			geometries[t.b].vel += massSolveB;
		}
	}

	void Update(float dt)
	{
		dt /= 3;
		for (int i = 0; i < 3; i++)
		{
			ApplyGravity(dt);
			lastStepCollisionInfo = DetectCollision();
			SolveCollision(lastStepCollisionInfo, dt);
			SolveOverlap(lastStepCollisionInfo, dt);
			for (auto& t : geometries)
			{
				t.transform.position += t.vel * dt;
			}
		}

		// Bounding obj in a 2x2 box
		/*for (auto& t : geometries)
		{
			Vector2 pos = t.transform.position;
			if (pos.y > 1)
			{
				t.transform.position.y -= 2;
			}
			else if (pos.y < -1)
			{
				t.transform.position.y += 2;
			}
		}*/
	}

	void RenderScene(int i, Shader* shader)
	{
		Geometry e = geometries[i];
		shader->SetUniformVec4("Color", e.color.r, e.color.g, e.color.b, e.color.a);
		shader->SetUniformVec2("Position", e.transform.position.x, e.transform.position.y);
		shader->SetUniformVec2("Size", e.transform.scale.x, e.transform.scale.y);
	}
};



void Scene::InitializeSceneTesting(int nOfTestEntities)
{
	for (int i = 0; i < nOfTestEntities; i++)
	{
		float randRadius = (rand()) / float(RAND_MAX) / 10.0 + 0.025;

		Geometry g(
			Transform2D(
				Vector2((rand()) / float(RAND_MAX) - 0.5f, (rand()) / float(RAND_MAX) - 0.5f) / 2.0,
				Vector2(randRadius, randRadius),
				0
			),
			EntityType::Circle
		);
		g.color = Color(0, 0.5, 1);
		g.vel = Vector2(
			(rand()) / float(RAND_MAX) - 0.5f,
			(rand()) / float(RAND_MAX) - 0.5f
		) * 16;
		g.restitution = 0.25;
		geometries.push_back(g);
	}
}

vector<CollisionTestInfo> Scene::SpatialSubdivisionTest()
{
	int size = geometries.size();

	float cellSize = 0; float lx = INFINITY, ly = INFINITY, hx = -INFINITY, hy = -INFINITY;
	for (auto& t : geometries)
	{
		float currentLargestSize = t.transform.scale.Max();
		if (currentLargestSize > cellSize) cellSize = currentLargestSize;

		Vector2 currentPos = t.transform.position;
		if (currentPos.x > hx) hx = currentPos.x;
		else if (currentPos.x < lx) lx = currentPos.x;

		if (currentPos.x > hy) hy = currentPos.x;
		else if (currentPos.x < ly) ly = currentPos.x;
	}
	cellSize *= 1;

	vector<vector<vector<Geometry>>> cells;
	vector<vector<Geometry>> subCells;
	vector<Geometry> singleCell;

	subCells.resize((hy - ly) / cellSize + 2, singleCell);
	cells.resize((hx - lx) / cellSize + 2, subCells);

	for (auto& t : geometries)
	{
		Vector2 tPos = t.transform.position;
		cells[(tPos.x - lx) / cellSize][(tPos.y - ly) / cellSize].push_back(t);
	}

	vector<CollisionTestInfo> SubdivisionCollisionList;

	// for every x-group cells (row)
	for (int i = 0; i < cells.size(); i++)
	{
		int rowSize = cells[i].size();

		// for every y-group cells (column)
		for (int j = 0; j < rowSize; j++)
		{
			if (j < rowSize - 1)
			{
				vector<Geometry> c0 = cells[i][j], c1 = cells[i][j + 1];

				for (int k = 0; k < c0.size(); k++)
				{
					for (int l = 0; l < c1.size(); l++)
					{
						if (Scene::AABBPairTest(c0[k], c1[l]))
						{
							SubdivisionCollisionList.push_back(
								CollisionTestInfo(
									k, l
								)
							);
						}
					}
				}
			}

			// Test for every objects in a single cells
			vector<CollisionTestInfo> currentCellTest = Scene::AABBCollisionsTest(cells[i][j]);

			SubdivisionCollisionList.insert(
				SubdivisionCollisionList.end(),
				currentCellTest.begin(),
				currentCellTest.end()
			);
		}
	}

	return SubdivisionCollisionList;
}