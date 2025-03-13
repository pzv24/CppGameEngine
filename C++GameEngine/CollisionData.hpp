#pragma once
#include "Entity.hpp"

class CollisionData
{
public:
	CollisionData(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
		:A(a), B(b)
	{
	}

	std::shared_ptr<Entity> A;
	std::shared_ptr<Entity> B;
	Vector2<float> normal{ 0,0 };
	float penDist = 0;
};

