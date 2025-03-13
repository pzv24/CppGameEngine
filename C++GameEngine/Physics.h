#pragma once
#include "EntityManager.h"
struct CollisionInfo
{
	Entity& A;
	Entity& B;
	float penetration;
	Vector2<float> hitNormal;
};

class Physics
{
public:
	bool collisionCirToCir(std::shared_ptr<Entity> A, std::shared_ptr<Entity> B);
	bool collisionCirToBox(std::shared_ptr<Entity> A, std::shared_ptr<Entity> B);
	bool collisionBoxToBox(std::shared_ptr<Entity> A, std::shared_ptr<Entity> B);
	bool isColliding(std::shared_ptr<Entity> A, std::shared_ptr<Entity> B);
};

