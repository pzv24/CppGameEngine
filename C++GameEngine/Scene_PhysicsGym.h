#pragma once
#include "Scene_Base.h"
#include "SInput.h"
#include "SMovement.h"
#include "Physics.h"
#include "Spawner.h"
#include <math.h>

class Scene_PhysicsGym : public Scene_Base
{
	//systems
	SInput m_sInput;
	SMovement m_sMovement;
	Physics m_physics;
	Spawner m_spawner;

	std::vector<CollisionData> m_collisions;

	//system methods
	void sRender(float alpha);
	void sDetectCollision(float dt);
	void drawImGui();

	//utility functions
	std::shared_ptr<Entity> getPlayer();

public:
	Scene_PhysicsGym(Game& game, float dt);
	void init();
	// Inherited via Scene_Base
	void sRender(float alpha) override;
	void sStep() override;
};

