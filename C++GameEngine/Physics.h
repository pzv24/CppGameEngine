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
	bool collisionCirToCir(CollisionData& collision);
	bool collisionCirToBox(CollisionData& collision);
	bool collisionBoxToBox(CollisionData& collision);
	bool isColliding(CollisionData& collision);
	void resolveCollision(CollisionData& collision);
	void bruteForceCorrection(CollisionData& collision);
};

