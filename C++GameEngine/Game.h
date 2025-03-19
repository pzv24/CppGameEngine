#pragma once
#include <iostream>
#include <memory>
#include <fstream>
#include "Vector2.hpp"

#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"

#include"EntityManager.h"
#include "SInput.h"
#include "SMovement.h"
#include "Physics.h"
#include <math.h>

class Game
{
	EntityManager m_entityManager;
	sf::Clock m_deltaClock;
	//will be using frames as unit of measurement instead of seconds, will use fixed frame rate for now
	int m_currentFrame = 0;
	bool m_paused;
	bool m_isRunning;
	int m_targetFPS = 60;
	sf::Time m_elapsed = sf::seconds(0);

	//systems

	//TO BE MOVED TO SCENE
	SInput m_sInput;
	SMovement m_sMovement;
	Physics m_physics;
	std::vector<CollisionData> m_collisions;
	void spawnPlayer();
	void spawnEnemyRect();
	void spawnEnemyCirc();
	std::shared_ptr<Entity> getPlayer();
	void sRender(float alpha);
	void sDetectCollision(float dt);
	void drawImGui();
	int randomRangeInt(int min, int max);
	int randomRangeFloat(float min, float max);

	void init(const std::string& configPath);
	void setPaused(bool paused);

	void sGetInput();

public:
	//window variables
	sf::RenderWindow m_gameWindow;
	sf::Font m_font;
	int m_wWidth = 1280;
	int m_wHeight = 720;

	Game(const std::string& configPath);
	void run();
};

