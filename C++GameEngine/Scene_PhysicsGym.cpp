#include "Scene_PhysicsGym.h"
#include "Game.h"

Scene_PhysicsGym::Scene_PhysicsGym(Game& game, float dt)
	:Scene_Base(game, dt)
{
}

void Scene_PhysicsGym::init()
{
}

void Scene_PhysicsGym::sStep()
{
}

void Scene_PhysicsGym::sDetectCollision(float dt)
{
}

void Scene_PhysicsGym::sRender(sf::RenderWindow& window, float alpha)
{
	window.clear();

	//first, do the frame counter
	sf::Text fpsCounter = sf::Text(m_game->m_font, "FPS", 12);
	fpsCounter.setString("FPS: " + std::to_string((int)(1.0f / m_elapsed.asSeconds())));
	fpsCounter.setPosition({ 0,0 });
	window.draw(fpsCounter);

	for (auto& e : m_entityManager.getEntities())
	{
		CTransform& transform = e->getComponent<CTransform>();
		//update the transform of the render component
		Vector2 interpolated = (transform.previousPosition * alpha) + (transform.position * (1.0f - alpha));
		//render the correct shape
		if (e->hasComponent<CCircle>())
		{
			e->getComponent<CCircle>().circle.setPosition(interpolated);
			window.draw(e->getComponent<CCircle>().circle);
		}
		if (e->hasComponent<CRectangle>())
		{
			e->getComponent<CRectangle>().rectangle.setPosition(interpolated);
			window.draw(e->getComponent<CRectangle>().rectangle);
		}
	}

	ImGui::SFML::Render(window);

	window.display();
}

void Scene_PhysicsGym::drawImGui()
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
				m_spawner.spawnEnemyRect(m_entityManager, m_game->m_wWidth, m_game->m_wHeight);
			}
			if (ImGui::Button("Spawn Enemy Circle"))
			{
				m_spawner.spawnEnemyCirc(m_entityManager, m_game->m_wWidth, m_game->m_wHeight);
			}
			if (ImGui::Button("Pasue"))
			{
				//setPaused(!m_paused);
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

std::shared_ptr<Entity> Scene_PhysicsGym::getPlayer()
{
	return nullptr;
	auto& players = m_entityManager.getEntities(EntityTag::player);

	std::cout << "There is " << players.size() << "players";
	if (players.size() > 1)
	{
		return players.front();
	}
}
