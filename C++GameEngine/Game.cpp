#include "Game.h"

Game::Game(const std::string& configPath)
{
	init(configPath);
}

void Game::init(const std::string& configPath)
{
	const int wWidth = 1280;
	const int wHeight = 720;
	m_gameWindow.create(sf::VideoMode({ wWidth, wHeight }), "GameScreen");
	m_gameWindow.setFramerateLimit(60);

	ImGui::SFML::Init(m_gameWindow);

	ImGui::GetStyle().ScaleAllSizes(1.0f);
	ImGui::GetIO().FontGlobalScale = 1.0f;
	m_isRunning = true;

	spawnPlayer();
}

void Game::setPaused(bool paused)
{
}

void Game::spawnPlayer()
{
	auto entity = m_entityManager.addEntity(player);

	entity->addComponent<CTransform>(Vector2(50.0f, 50.0f), 0.0f, Vector2(1.0f, 1.0f));
	entity->addComponent<CRigidbody>();
	entity->addComponent<CCircle>(CCircle(5.0f, 32, sf::Color(120,120,255), sf::Color(255,255,255), 1.0f));
	entity->addComponent<CCircleCollider>(5.0f);
	entity->getComponent<CInput>();
}

void Game::run()
{
	while (m_isRunning)
	{
		m_entityManager.update();

		ImGui::SFML::Update(m_gameWindow, m_deltaClock.restart());

		//loop the systems
		//m_sInput.update();
		while (const std::optional event = m_gameWindow.pollEvent())
		{
			ImGui::SFML::ProcessEvent(m_gameWindow, *event);

			if (event->is<sf::Event::Closed>())
			{
				m_gameWindow.close();
			}

			if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
			{
				std::cout << "Key pressed with code = " << static_cast<int>(keyPressed->scancode);
			}
		}
		//m_sMovement.update();
		//m_sCollision.update();
		drawImGui();
		sRender();

		m_currentFrame++;
	}
}

std::shared_ptr<Entity> Game::getPlayer()
{
	auto& players = m_entityManager.getEntities(EntityTag::player);
	return players.front();
}

void Game::sRender()
{
	m_gameWindow.clear();

	getPlayer()->getComponent<CCircle>().circle.setPosition(getPlayer()->getComponent<CTransform>().position);
	//getPlayer()->getComponent<CTransform>().rotation += 1;
	m_gameWindow.draw(getPlayer()->getComponent<CCircle>().circle);

	ImGui::SFML::Render(m_gameWindow);

	m_gameWindow.display();
}

void Game::drawImGui()
{
	ImGui::Begin("GUI");
	ImGui::Text("Window Text");
	ImGui::End();
}
