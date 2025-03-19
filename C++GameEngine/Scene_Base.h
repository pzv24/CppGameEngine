#pragma once
#include "EntityManager.h"

class Game;

class Scene_Base
{
protected:
	Game* m_game;
	EntityManager m_entityManager;
	sf::Time m_elapsed = sf::seconds(0);
	bool m_paused;
	float m_dt;
	//add information of input action mapping

public:
	Scene_Base(Game& game, float dt);
	virtual void sRender(float alpha) = 0;
	virtual void sStep() = 0;
};

