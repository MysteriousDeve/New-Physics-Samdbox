#pragma once

#include "../ProjectInclude.h"
#include "../Renderer/Shader.h"
#include "PhysicsSolverFunc.h"

using namespace std;

class Scene
{
public:
	vector<EntityData::Geom> geometries;
	vector<EntityData::EntityProp> entityProps;
	vector<Entity> entities;

	Vector2 gravity = Vector2(0, -10);

	// Count the existing entities available in scene
	int entityCount = 0, geometryCount = 0;

	// Default constructor
	Scene() {}

	p_dec simUpdateDelta = 1 / 60.0;

	void AddEntity(EntityType type)
	{
		entities.push_back(Entity(type, entities.size(), { -1, -1, -1 }));
		entityCount++;
		return;
	}
	Entity *AddGeometry(GeometryType type)
	{
		Entity e = Entity(EntityType::Geometry, entities.size(), { (int)geometries.size(), -1, -1 });
		entities.push_back(e);
		geometries.push_back(EntityData::Geom());
		entityCount++;
		geometryCount++;

		return &entities[entityCount - 1];
	}
	
	void InitializeSceneTesting(int nOfTestEntities)
	{
		for (int i = 0; i < nOfTestEntities; i++)
		{
			float randRadius = (rand()) / float(RAND_MAX) / 10.0 + 0.025;

			AddGeometry(GeometryType::Circle);

			Geometry g(
				Transform2D(
					Vector2((rand()) / float(RAND_MAX) - 0.5f, (rand()) / float(RAND_MAX) - 0.5f) / 2.0,
					Vector2(randRadius, randRadius),
					0
				),
				EntityType::Circle
			);
			g.vel = Vector2(
				(rand()) / float(RAND_MAX) - 0.5f,
				(rand()) / float(RAND_MAX) - 0.5f
			) * 16;
			g.restitution = 0.25;
			geometries.push_back(g);
		}
	}

	EntityData::Geom* GetGeometryRef(int i)
	{
		return &geometries[i];
	}

	Entity* GetEntityRef(int i)
	{
		return &entities[i];
	}

	// Apply gravity
	void ApplyGravity(float dt)
	{
		for (auto& x : geometries)
		{
			x.vel += gravity * dt * (x.density != INFINITY) * x.gravityScale;
		}
	}

	// Collisions detection
	vector<CollisionTestInfo> SpatialSubdivisionTest();
	vector<CollisionTestInfo> AABBCollisionsTest(vector<EntityData::Geom> geometries)
	{
		int size = geometries.size();
		vector<CollisionTestInfo> testList;
		for (int i = 0; i < size; i++)
		{
			for (int j = i + 1; j < size; j++)
			{
				bool cond = AABBPairTestDebug(geometries[i], geometries[j]);
				if (cond)
				{
					testList.push_back(CollisionTestInfo(geometries[i], geometries[j]));
				}
			}
		}
		return testList;
	}

	bool AABBPairTest(EntityData::Geom a, EntityData::Geom b)
	{
		return AABB::DetectCollisionTest(a.aabb, b.aabb);
	}

	bool AABBPairTestDebug(EntityData::Geom a, EntityData::Geom b)
	{
		bool cond = AABB::DetectCollisionTest(a.aabb, b.aabb);
		return cond;
	}

	vector<CollisionInfo> NarrowCollisionsTest(vector<CollisionTestInfo> testList)
	{
		int size = geometries.size();
		vector<CollisionInfo> outputList;

		for (auto &t: testList)
		{
			float depth = pow((t.a->transform.scale.x + t.b->transform.scale.x) / 2.0f, 2) - (a->transform.position - b->transform.position).lenSq();
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
			geometries[i].CalculateAABB();
		}

		return NarrowCollisionsTest(AABBCollisionsTest(geometries));
		//NarrowCollisionsTest(
		//	SpatialSubdivisionTest()
		//);
		
	}

	void SolveCollision(vector<CollisionWrapper> info)
	{
		for (auto &x : info)
		{
			Solve(info, simUpdateDelta);
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
	void PhysicsUpdate(float dt)
	{
		for (auto& t : geometries)
		{
			t.transform.position += t.vel * dt;
		}
	}

	void Update()
	{
		dt /= 3;
		for (int i = 0; i < 3; i++)
		{
			ApplyGravity();
			auto currentCollisionInfo = DetectCollision();
			SolveCollision(currentCollisionInfo, dt);
			SolveOverlap(currentCollisionInfo, dt);
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
		Entity e = entities[i];
		shader->SetUniformVec4("Color", e.color.r, e.color.g, e.color.b, e.color.a);
		shader->SetUniformVec2("Position", e.geom.transform.position.x, e.geom.transform.position.y);
		shader->SetUniformVec2("Size", e.bound.x, e.bound.y);
	}
};

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

	vector<vector<vector<EntityData::Geom>>> cells;
	vector<vector<EntityData::Geom>> subCells;
	vector<EntityData::Geom> singleCell;

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
				vector<EntityData::Geom> c0 = cells[i][j], c1 = cells[i][j + 1];

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