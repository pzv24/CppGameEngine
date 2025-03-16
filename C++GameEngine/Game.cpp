#include "Game.h"

sf::Time getCurrentTime()
{
	static sf::Clock clock;
	return clock.getElapsedTime();
}

Game::Game(const std::string& configPath)
{
	init(configPath);
}

void Game::init(const std::string& configPath)
{
	m_gameWindow.create(sf::VideoMode({ (unsigned)m_wWidth, (unsigned)m_wHeight }), "GameScreen");
	m_gameWindow.setFramerateLimit(m_targetFPS);
	m_deltaClock.restart();
	ImGui::SFML::Init(m_gameWindow);

	ImGui::GetStyle().ScaleAllSizes(1.0f);
	ImGui::GetIO().FontGlobalScale = 1.0f; 

	if (!m_font.openFromFile("./assets/fonts/small_pixel.ttf"))
	{
		std::cerr << "Could not load font! \n";
		exit(0);
	}

	m_isRunning = true;
	srand(time(NULL));
	spawnPlayer();
}

void Game::setPaused(bool paused)
{
	m_paused = paused;
}

void Game::spawnPlayer()
{
	auto entity = m_entityManager.addEntity(player);

	entity->addComponent<CTransform>(Vector2(50.0f, 50.0f), 0.0f, Vector2(1.0f, 1.0f));
	entity->addComponent<CRigidbody>(Vector2{0.0f,0.0f}, 0.0f, 10.0f);

	entity->addComponent<CRectangle>((CRectangle(Vector2{ 30.0f,30.0f }, sf::Color(120, 50, 255), sf::Color(255, 255, 255), 1.0f)));
	entity->addComponent<CBoxCollider>((Vector2{ 30.0f,30.0f }));
	//entity->addComponent<CCircle>(CCircle(15.0f, 32, sf::Color(120,50,255), sf::Color(255,255,255), 1.0f));
	//entity->addComponent<CCircleCollider>(15.0f);

	entity->getComponent<CInput>();
}

void Game::spawnEnemyRect()
{
	auto entity = m_entityManager.addEntity(enemy);
	float randomAngle = randomRangeInt(0, 360);
	randomAngle = randomAngle * 3.141692 / 180;
	float randomSpeed = randomRangeFloat(100.0f, 250.0f);
	Vector2<float> randomVel(cos(randomAngle) * randomSpeed, sin(randomAngle) * randomSpeed);
	Vector2<float> randomPos(randomRangeInt(15, m_wWidth - 15), randomRangeInt(15, m_wHeight - 15));
	sf::Color randomColor(randomRangeInt(0, 255), randomRangeInt(0, 255), randomRangeInt(0, 255));

	entity->addComponent<CTransform>(randomPos, 0.0f, Vector2(1.0f, 1.0f));
	entity->addComponent<CRigidbody>(randomVel,0.0f, 4.0f);

	entity->addComponent<CRectangle>((CRectangle(Vector2{80.0f,80.0f}, randomColor, sf::Color(255, 255, 255), 1.0f)));
	entity->addComponent<CBoxCollider>((Vector2{ 80.0f,80.0f }));
}

void Game::spawnEnemyCirc()
{
	auto entity = m_entityManager.addEntity(enemy);
	float randomAngle = randomRangeInt(0, 360);
	randomAngle = randomAngle * 3.141692 / 180;
	float randomSpeed = randomRangeFloat(80.0f, 180.0f);
	Vector2<float> randomVel(cos(randomAngle) * randomSpeed, sin(randomAngle) * randomSpeed);
	Vector2<float> randomPos(randomRangeInt(15, m_wWidth - 15), randomRangeInt(15, m_wHeight - 15));
	sf::Color randomColor(randomRangeInt(0, 255), randomRangeInt(0, 255), randomRangeInt(0, 255));

	entity->addComponent<CTransform>(randomPos, 0.0f, Vector2(1.0f, 1.0f));
	entity->addComponent<CRigidbody>(randomVel, 0.0f, 4.0f);

	entity->addComponent<CCircle>(CCircle(25.0f, 32, randomColor, sf::Color(255, 255, 255), 1.0f));
	entity->addComponent<CCircleCollider>(25.0f);
}

void Game::run()
{
	// in milliseconds, about 16.6 for 60 FPS
	float dt = 1.0f / (m_targetFPS*2.0f);
	float lag = 0.0f;

	float frameStart = getCurrentTime().asSeconds();
	while (m_isRunning)
	{
		float currentTime = getCurrentTime().asSeconds();
		m_elapsed = sf::seconds(currentTime - frameStart);
		//std::cout << "lag = " << m_elapsed.asSeconds() << " while dt is: " << dt << std::endl;
		lag += currentTime - frameStart;
		frameStart = currentTime;

		//update imgui
		ImGui::SFML::Update(m_gameWindow, m_deltaClock.restart());

		//loop the systems
		//fist, update input
		handleInput();

		//update the lists on the entity manager before we go into physics loop
		m_entityManager.update();

		// safety break in case physics update takes longer than the dt
		if (lag > 0.25f)
		{
			lag = 0.25f;
		}

		//iterate through the gameplay systems as many times as available
		int count = 0;
		while (lag > dt)
		{
			//main game updates
			sDetectCollision(dt);
			for (auto& c : m_collisions)
			{
				m_physics.resolveCollision(c);
			}
			for (auto& c : m_collisions)
			{
				m_physics.bruteForceCorrection(c);
			}
			m_sMovement.update(m_entityManager, m_gameWindow, dt);
			lag -= dt;
			++count;
		}
		std::cout << count << std::endl;
		// calculate the lag "leftover" by the dt amount
		const float alpha = lag / dt;
		
		drawImGui();
		sRender(alpha);

		//sf::sleep(sf::seconds(frameStart + (1.0f/m_targetFPS) - getCurrentTime().asSeconds()));
	}
}

std::shared_ptr<Entity> Game::getPlayer()
{
	return nullptr;
	auto& players = m_entityManager.getEntities(EntityTag::player);
	
	std::cout << "There is " << players.size() << "players";
	if (players.size() > 1)
	{
		return players.front();
	}
}

void Game::sRender(float alpha)
{
	m_gameWindow.clear();

	//first, do the frame counter
	sf::Text fpsCounter = sf::Text(m_font, "FPS", 12);
	fpsCounter.setString("FPS: " + std::to_string((int)(1.0f/ m_elapsed.asSeconds())));
	fpsCounter.setPosition({ 0,0 });
	m_gameWindow.draw(fpsCounter);

	for (auto& e : m_entityManager.getEntities())
	{
		CTransform& transform = e->getComponent<CTransform>();
		//update the transform of the render component
		Vector2 interpolated = (transform.previousPosition * alpha) + (transform.position * (1.0f - alpha));
		//render the correct shape
		if (e->hasComponent<CCircle>())
		{
			e->getComponent<CCircle>().circle.setPosition(interpolated);
			m_gameWindow.draw(e->getComponent<CCircle>().circle);
		}
		if (e->hasComponent<CRectangle>())
		{
			e->getComponent<CRectangle>().rectangle.setPosition(interpolated);
			m_gameWindow.draw(e->getComponent<CRectangle>().rectangle);
		}
	}

	ImGui::SFML::Render(m_gameWindow);

	m_gameWindow.display();
}

void Game::sDetectCollision(float dt)
{
	m_collisions.clear();
	for (auto& a : m_entityManager.getEntities(enemy))
	{
		for (auto& b : m_entityManager.getEntities(enemy))
		{
			CollisionData c(a, b);
			if (m_physics.isColliding(c))
			{
				m_collisions.emplace_back(c);
				//std::cout << "Frame no. " << m_currentFrame << "Collission between entity ID : " << a->getId() << " and ID : " << b->getId() << std::endl;
			}
		}
	}
}

void Game::handleInput()
{
	while (const std::optional event = m_gameWindow.pollEvent())
	{
		ImGui::SFML::ProcessEvent(m_gameWindow, *event);

		// if window is closed, exit the game
		if (event->is<sf::Event::Closed>())
		{
			m_isRunning = false;
		}

		// before processing input for gameplay, check if escape key was pressed and exit the game
		if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
		{
			if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
			{
				m_isRunning = false;
				std::cout << "Exiting game...";
				return;
			}
		}

		// else, pass over the event to the input system
		m_sInput.update(m_entityManager, *event);
	}
}

void Game::drawImGui()
{
	ImGui::Begin("GUI");
	ImGui::ShowDemoWindow();
	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
	{
		if (ImGui::BeginTabItem("Home"))
		{
			if (ImGui::Button("Spawn Enemy Rectangle"))
			{
				spawnEnemyRect();
			}
			if (ImGui::Button("Spawn Enemy Circle"))
			{
				spawnEnemyCirc();
			}
			if (ImGui::Button("Pasue"))
			{
				setPaused(!m_paused);
			}

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Entities"))
		{
			if (ImGui::CollapsingHeader("All Entities", ImGuiTreeNodeFlags_None))
			{
				if (ImGui::BeginTable("table1", 4))
				{
					ImGui::TableSetupColumn("DeleteEntity");
					ImGui::TableSetupColumn("ID");
					ImGui::TableSetupColumn("Tag");
					ImGui::TableSetupColumn("Pos");
					ImGui::TableHeadersRow();

					for (auto& e : m_entityManager.getEntities())
					{
						ImGui::TableNextRow();

						ImGui::TableSetColumnIndex(0);
						ImGui::PushID(e->getId());
						if (ImGui::Button("DEL"))
						{
							e->destroyEntity();
						}
						ImGui::PopID();

						ImGui::TableSetColumnIndex(1);
						ImGui::Text(e->getTagName());

						ImGui::TableSetColumnIndex(2);
						ImGui::Text("%d", e->getId());

						ImGui::TableSetColumnIndex(3);
						Vector2<float> vec = e->getComponent<CTransform>().position;
						ImGui::Text("(%d", static_cast<int>(vec.x)); ImGui::SameLine();
						ImGui::Text(","); ImGui::SameLine();
						ImGui::Text("%d)", static_cast<int>(vec.y)); ImGui::SameLine();
					}
					ImGui::EndTable();
				}
			}
			if (ImGui::CollapsingHeader("Entities by tag", ImGuiTreeNodeFlags_None))
			{
				if (ImGui::CollapsingHeader("enemy", ImGuiTreeNodeFlags_None))
				{
					if (ImGui::BeginTable("table2", 4))
					{
						ImGui::TableSetupColumn("DeleteEntity");
						ImGui::TableSetupColumn("ID");
						ImGui::TableSetupColumn("Tag");
						ImGui::TableSetupColumn("Pos");
						ImGui::TableHeadersRow();

						for (auto& e : m_entityManager.getEntities(enemy))
						{
							ImGui::TableNextRow();

							ImGui::TableSetColumnIndex(0);
							ImGui::PushID(e->getId());
							if (ImGui::Button("DEL"))
							{
								e->destroyEntity();
							}
							ImGui::PopID();

							ImGui::TableSetColumnIndex(1);
							ImGui::Text(e->getTagName());

							ImGui::TableSetColumnIndex(2);
							ImGui::Text("%d", e->getId());

							ImGui::TableSetColumnIndex(3);
							Vector2<float> vec = e->getComponent<CTransform>().position;
							ImGui::Text("(%d", static_cast<int>(vec.x)); ImGui::SameLine();
							ImGui::Text(","); ImGui::SameLine();
							ImGui::Text("%d)", static_cast<int>(vec.y)); ImGui::SameLine();
						}
						ImGui::EndTable();
					}
				}
				if (ImGui::CollapsingHeader("player", ImGuiTreeNodeFlags_None))
				{
					if (ImGui::BeginTable("table3", 4))
					{
						ImGui::TableSetupColumn("DeleteEntity");
						ImGui::TableSetupColumn("ID");
						ImGui::TableSetupColumn("Tag");
						ImGui::TableSetupColumn("Pos");
						ImGui::TableHeadersRow();

						for (auto& e : m_entityManager.getEntities(player))
						{
							ImGui::TableNextRow();

							ImGui::TableSetColumnIndex(0);
							ImGui::PushID(e->getId());
							if (ImGui::Button("DEL"))
							{
								e->destroyEntity();
							}
							ImGui::PopID();

							ImGui::TableSetColumnIndex(1);
							ImGui::Text(e->getTagName());

							ImGui::TableSetColumnIndex(2);
							ImGui::Text("%d", e->getId());

							ImGui::TableSetColumnIndex(3);
							Vector2<float> vec = e->getComponent<CTransform>().position;
							ImGui::Text("(%d", static_cast<int>(vec.x)); ImGui::SameLine();
							ImGui::Text(","); ImGui::SameLine();
							ImGui::Text("%d)", static_cast<int>(vec.y)); ImGui::SameLine();
						}
						ImGui::EndTable();
					}
				}
			}
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::Separator();
	ImGui::End();
}

int Game::randomRangeInt(int min, int max)
{
	return min + (rand() % (1 + max - min));
}

int Game::randomRangeFloat(float min, float max)
{
	return  min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}
