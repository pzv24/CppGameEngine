#include "SMovement.h"

void SMovement::update(EntityManager& entityManager)
{
	//for entities with input (player) apply their input to their speed variable
	CInput& playerInput = entityManager.getEntities(EntityTag::player).front()->getComponent<CInput>();
	CRigidbody& playerRB = entityManager.getEntities(EntityTag::player).front()->getComponent<CRigidbody>();
	Vector2<float> inputVector{};
	if (playerInput.down) inputVector.y += +1.0f;
	if (playerInput.up) inputVector.y += -1.0f;
	if (playerInput.left) inputVector.x += -1.0f;
	if (playerInput.right) inputVector.x += 1.0f;
	inputVector = inputVector.normalized();
	inputVector *= 5;
	playerRB.velocity = inputVector;

	//for all entities
	for (auto& e : entityManager.getEntities())
	{
		if (e->hasComponent<CTransform>() && e->hasComponent<CRigidbody>())
		{
			e->getComponent<CTransform>().position += e->getComponent<CRigidbody>().velocity;
		}
	}
}
