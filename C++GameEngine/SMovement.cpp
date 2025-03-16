#include "SMovement.h"

void SMovement::update(EntityManager& entityManager, sf::RenderWindow& window, float dt)
{
	//for entities with input (player) apply their input to their speed variable
	if (entityManager.hasPlayer())
	{
		CInput& playerInput = entityManager.getEntities(EntityTag::player).front()->getComponent<CInput>();
		CRigidbody& playerRB = entityManager.getEntities(EntityTag::player).front()->getComponent<CRigidbody>();
		Vector2<float> inputVector{};
		if (playerInput.down) inputVector.y += 1.0f;
		if (playerInput.up) inputVector.y += -1.0f;
		if (playerInput.left) inputVector.x += -1.0f;
		if (playerInput.right) inputVector.x += 1.0f;
		inputVector = inputVector.normalized();
		inputVector *= 500.0f;
		playerRB.velocity = inputVector;
	}

	//for all entities
	for (auto& e : entityManager.getEntities())
	{
		if (e->hasComponent<CTransform>() && e->hasComponent<CRigidbody>())
		{
			CTransform& transform = e->getComponent<CTransform>();
			CRigidbody& rb = e->getComponent<CRigidbody>();

			transform.previousPosition = transform.position;
			transform.position += rb.velocity * dt;

			// if the player is going off bounds, restrain him
			if (e->getTag() == player)
			{
				CCircleCollider& collider = e->getComponent<CCircleCollider>();
				if (transform.position.x + collider.radius > window.getSize().x ||
					transform.position.x < collider.radius)
				{
					transform.position.x = transform.position.x - rb.velocity.x * dt;
					rb.velocity.x = 0;
				}
				if (transform.position.y + collider.radius > window.getSize().y ||
					transform.position.y < collider.radius)
				{
					transform.position.y = transform.position.y - rb.velocity.y *dt;
					rb.velocity.y = 0;
				}
				transform.previousPosition = transform.position;
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
				CBoxCollider& rectCollider = e->getComponent<CBoxCollider>();
				if (transform.position.x + rectCollider.halfSize.x > window.getSize().x ||
					transform.position.x < rectCollider.halfSize.x)
				{
					rb.velocity.x *= -1;
				}
				if (transform.position.y + rectCollider.halfSize.y > window.getSize().y ||
					transform.position.y < rectCollider.halfSize.y)
				{
					rb.velocity.y *= -1;
				}
			}
		}
	}
}
