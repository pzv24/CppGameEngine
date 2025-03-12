#include "Game.h"

Game::Game(const std::string& configPath)
{
	init(configPath);
}

void Game::init(const std::string& configPath)
{
	m_gameWindow.create(sf::VideoMode({ (unsigned)m_wWidth, (unsigned)m_wHeight }), "GameScreen");
	m_gameWindow.setFramerateLimit(60);

	ImGui::SFML::Init(m_gameWindow);

	ImGui::GetStyle().ScaleAllSizes(1.0f);
	ImGui::GetIO().FontGlobalScale = 1.0f;
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
	entity->addComponent<CRigidbody>();
	entity->addComponent<CCircle>(CCircle(15.0f, 32, sf::Color(120,50,255), sf::Color(255,255,255), 1.0f));
	entity->addComponent<CCircleCollider>(15.0f);
	entity->getComponent<CInput>();
}

void Game::spawnEnemy()
{
	auto entity = m_entityManager.addEntity(enemy);
	float randomAngle = randomRangeInt(0, 360);
	randomAngle = randomAngle * 3.141692 / 180;
	float randomSpeed = randomRangeFloat(3.0f, 25.0f);
	Vector2<float> randomVel(cos(randomAngle) * randomSpeed, sin(randomAngle) * randomSpeed);
	Vector2<float> randomPos(randomRangeInt(15, m_wWidth - 15), randomRangeInt(15, m_wHeight - 15));
	sf::Color randomColor(randomRangeInt(0, 255), randomRangeInt(0, 255), randomRangeInt(0, 255));

	entity->addComponent<CTransform>(randomPos, 0.0f, Vector2(1.0f, 1.0f));
	entity->addComponent<CRigidbody>().velocity = randomVel;
	entity->addComponent<CCircle>(CCircle(15.0f, 6, randomColor, sf::Color(255, 255, 255), 1.0f));
	entity->addComponent<CCircleCollider>(15.0f);
}

void Game::run()
{
	while (m_isRunning)
	{
		m_entityManager.update();

		ImGui::SFML::Update(m_gameWindow, m_deltaClock.restart());

		//loop the systems
		//update input
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
		if (!m_paused)
		{
			m_sMovement.update(m_entityManager, m_gameWindow);
			sDetectCollision();
			m_currentFrame++;
		}
		drawImGui();
		sRender();
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

void Game::sRender()
{
	m_gameWindow.clear();

	for (auto& e : m_entityManager.getEntities())
	{
		if (e->hasComponent<CCircle>())
		{
			e->getComponent<CCircle>().circle.setPosition(e->getComponent<CTransform>().position);
			m_gameWindow.draw(e->getComponent<CCircle>().circle);
		}
	}

	ImGui::SFML::Render(m_gameWindow);

	m_gameWindow.display();
}

void Game::sDetectCollision()
{
	for (auto& e : m_entityManager.getEntities(enemy))
	{
		for (auto& p : m_entityManager.getEntities(player))
		{
			if (p->getComponent<CTransform>().position.sqrdDistance(e->getComponent<CTransform>().position) <
				(p->getComponent<CCircleCollider>().radius + e->getComponent<CCircleCollider>().radius) *
				(p->getComponent<CCircleCollider>().radius + e->getComponent<CCircleCollider>().radius))
			{
				p->destroyEntity();
			}
		}
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
			if (ImGui::Button("Spawn Enemy"))
			{
				spawnEnemy();
			}
			ImGui::SameLine();
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
