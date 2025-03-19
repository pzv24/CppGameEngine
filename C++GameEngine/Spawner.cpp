#include "Spawner.h"

std::shared_ptr<Entity> Spawner::spawnPlayer(EntityManager& entityManager)
{
	auto entity = entityManager.addEntity(player);

	entity->addComponent<CTransform>(Vector2(50.0f, 50.0f), 0.0f, Vector2(1.0f, 1.0f));
	entity->addComponent<CRigidbody>(Vector2{ 0.0f,0.0f }, 0.0f, 10.0f);

	entity->addComponent<CRectangle>((CRectangle(Vector2{ 30.0f,30.0f }, sf::Color(120, 50, 255), sf::Color(255, 255, 255), 1.0f)));
	entity->addComponent<CBoxCollider>((Vector2{ 30.0f,30.0f }));
	//entity->addComponent<CCircle>(CCircle(15.0f, 32, sf::Color(120,50,255), sf::Color(255,255,255), 1.0f));
	//entity->addComponent<CCircleCollider>(15.0f);

	entity->getComponent<CInput>();
}

std::shared_ptr<Entity> Spawner::spawnEnemyRect(EntityManager& entityManager, int screenW, int screenH)
{
	auto entity = entityManager.addEntity(enemy);
	float randomAngle = RandomFuncs::randomRangeInt(0, 360);
	randomAngle = randomAngle * 3.141692 / 180;
	float randomSpeed = RandomFuncs::randomRangeFloat(100.0f, 250.0f);
	Vector2<float> randomVel(cos(randomAngle) * randomSpeed, sin(randomAngle) * randomSpeed);
	Vector2<float> randomPos(RandomFuncs::randomRangeInt(15, screenW - 15), RandomFuncs::randomRangeInt(15, screenH - 15));
	sf::Color randomColor(RandomFuncs::randomRangeInt(0, 255), RandomFuncs::randomRangeInt(0, 255), RandomFuncs::randomRangeInt(0, 255));

	entity->addComponent<CTransform>(randomPos, 0.0f, Vector2(1.0f, 1.0f));
	entity->addComponent<CRigidbody>(randomVel, 0.0f, 4.0f);

	entity->addComponent<CRectangle>((CRectangle(Vector2{ 80.0f,80.0f }, randomColor, sf::Color(255, 255, 255), 1.0f)));
	entity->addComponent<CBoxCollider>((Vector2{ 80.0f,80.0f }));
}

std::shared_ptr<Entity> Spawner::spawnEnemyCirc(EntityManager& entityManager, int screenW, int screenH)
{
	auto entity = entityManager.addEntity(enemy);
	float randomAngle = RandomFuncs::randomRangeInt(0, 360);
	randomAngle = randomAngle * 3.141692 / 180;
	float randomSpeed = RandomFuncs::randomRangeFloat(80.0f, 180.0f);
	Vector2<float> randomVel(cos(randomAngle) * randomSpeed, sin(randomAngle) * randomSpeed);
	Vector2<float> randomPos(RandomFuncs::randomRangeInt(15, screenW - 15), RandomFuncs::randomRangeInt(15, screenH - 15));
	sf::Color randomColor(RandomFuncs::randomRangeInt(0, 255), RandomFuncs::randomRangeInt(0, 255), RandomFuncs::randomRangeInt(0, 255));

	entity->addComponent<CTransform>(randomPos, 0.0f, Vector2(1.0f, 1.0f));
	entity->addComponent<CRigidbody>(randomVel, 0.0f, 4.0f);

	entity->addComponent<CCircle>(CCircle(25.0f, 32, randomColor, sf::Color(255, 255, 255), 1.0f));
	entity->addComponent<CCircleCollider>(25.0f);
}
