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
#include <math.h>

class Game
{
	EntityManager m_entityManager;
	sf::RenderWindow m_gameWindow;
	sf::Font m_font;
	sf::Clock m_deltaClock;

	//will be using frames as unit of measurement instead of seconds, will use fixed frame rate for now
	int m_currentFrame = 0;
	bool m_paused;
	bool m_isRunning;
	int m_wWidth = 1280;
	int m_wHeight = 720;

	//systems
	SInput m_sInput;
	SMovement m_sMovement;

	void init(const std::string& configPath);
	void setPaused(bool paused);
	void spawnPlayer();
	void spawnEnemy();

	std::shared_ptr<Entity> getPlayer();
	void sRender();
	void sDetectCollision();
	void drawImGui();
	int randomRangeInt(int min, int max);
	int randomRangeFloat(float min, float max);

public:
	Game(const std::string& configPath);
	void run();
};

