#pragma once
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <limits>
#include <iterator>

#include "Vector2.h"
#include "Box.h"
#include "Collision.h"
#include "cute_c2.h"
class World
{
public:
	World();
	~World();

	void(*OnCreate)(Box* box);
	std::map<std::string, Box*> Boxes;
	
	Box* World::Create(float x, float y, float width, float height, bool isStatic, const std::string& name);
	void World::Destroy(const std::string& name);
	Box* World::GetSweptBroadphaseBox(Box& b);
	bool World::AABBCheck(Box& b1, Box& b2);
	void World::DefaultCollision(Box* box, Box* other, c2Manifold& m);
	Direction World::StepBox(Box* box);
	int World::StepSimulation(std::vector<Collision*>& colls);

};