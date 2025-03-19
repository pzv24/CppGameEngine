#pragma once
#include "EntityManager.h"
#include "RandomFuncs.hpp"
class Spawner
{
public:
	Spawner() = default;

	std::shared_ptr<Entity> spawnPlayer(EntityManager& entityManager);
	std::shared_ptr<Entity> spawnEnemyRect(EntityManager& entityManager, int screenW = 0, int screenH = 0);
	std::shared_ptr<Entity> spawnEnemyCirc(EntityManager& entityManager, int screenW = 0, int screenH = 0);


};

