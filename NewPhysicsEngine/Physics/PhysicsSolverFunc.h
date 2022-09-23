#pragma once

#include "../ProjectInclude.h"

#include "Entity.h"

const void CircleCircle(Entity a, Entity b)
{
	float massA = a.GetMass();
	float massB = b.GetMass();

	Vector2 velA = geomA.GetVelocity();
	Vector2 velB = geomB.GetVelocity();

	float minRestitution = std::min(geomA.restitution, geomB.restitution);
	float totalMass = massA + massB;

	geometries[t.a].vel = (velA * (massA - massB) + velB * massB * 2) / totalMass * minRestitution;
	geometries[t.b].vel = (velB * (massB - massA) + velA * massA * 2) / totalMass * minRestitution;
}

constexpr void CirclePlane()
{

}

const std::function<void()> funcList[]
{
	&CircleCircle,
	&CirclePlane
};

const std::function<void()> GetFunc(int i)
{
	return funcList[i];
}