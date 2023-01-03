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

	p_dec simUpdateDelta = 1 / 60.0;
	p_dec simUpdateDeltaInternal = 1 / 60.0;

	// Default constructor
	Scene() 
	{

	}

	Scene(p_dec simUpdateDelta)
	{
		this->simUpdateDelta = simUpdateDelta;
		simUpdateDeltaInternal = simUpdateDelta;
	}

	void AddEntity(EntityType type)
	{
		entities.push_back(Entity(type, entities.size(), { -1, -1, -1 }));
		entityCount++;
		return;
	}

	Entity* AddGeometry(GeometryType type)
	{
		Entity e = Entity(EntityType::Geometry, entities.size(), { (int)geometries.size(), -1, -1 });
		entities.push_back(e);
		geometries.push_back(EntityData::Geom());
		entityCount++;
		geometryCount++;

		return &entities[entityCount - 1];
	}
	EntityData::Geom* GetGeomFromEntity(Entity* entity)
	{
		return &geometries[entity->entityRef[0]];
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