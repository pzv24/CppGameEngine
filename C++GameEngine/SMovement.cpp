#include "SMovement.h"

void SMovement::update(EntityManager& entityManager, sf::RenderWindow& window)
{
	//for entities with input (player) apply their input to their speed variable
	if (entityManager.hasPlayer())
	{
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
	}

	//for all entities
	for (auto& e : entityManager.getEntities())
	{
		if (e->hasComponent<CTransform>() && e->hasComponent<CRigidbody>())
		{
			CTransform& transform = e->getComponent<CTransform>();
			CRigidbody& rb = e->getComponent<CRigidbody>();
			transform.position += rb.velocity;

			// if the player is going off bounds, restrain him
			if (e->getTag() == player)
			{
				CCircleCollider& collider = e->getComponent<CCircleCollider>();
				if (transform.position.x + collider.radius > window.getSize().x ||
					transform.position.x < collider.radius)
				{
					transform.position.x = transform.position.x - rb.velocity.x;
					rb.velocity.x = 0;
				}
				if (transform.position.y + collider.radius > window.getSize().y ||
					transform.position.y < collider.radius)
				{
					transform.position.y = transform.position.y - rb.velocity.y;
					rb.velocity.y = 0;
				}
			}
			// if enemies are going off bounds, bounce them
			if (e->getTag() == enemy)
			{
				CCircleCollider& collider = e->getComponent<CCircleCollider>();
				if (transform.position.x + collider.radius > window.getSize().x ||
					transform.position.x < collider.radius)
				{
					rb.velocity.x *= -1;
				}
				if (transform.position.y + collider.radius > window.getSize().y ||
					transform.position.y < collider.radius)
				{
					rb.velocity.y *= -1;
				}
			}
		}
	}
}
