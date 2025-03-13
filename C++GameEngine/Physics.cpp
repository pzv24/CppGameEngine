#include "Physics.h"

bool Physics::collisionCirToCir(std::shared_ptr<Entity> A, std::shared_ptr<Entity> B)
{
	//if A does not have Circle Collider, or B does not have circle collider, or A and B are the same entity, return false
	if (!A->hasComponent<CCircleCollider>() || !B->hasComponent<CCircleCollider>() || A==B) return false;

	//calculate the sum of radius
	float radiusSum = A->getComponent<CCircleCollider>().radius + B->getComponent<CCircleCollider>().radius;

	// compare squared radius with squared distance
	return radiusSum * radiusSum > A->getComponent<CTransform>().position.sqrdDistance(B->getComponent<CTransform>().position);
}

bool Physics::collisionCirToBox(std::shared_ptr<Entity> cirEnt, std::shared_ptr<Entity> boxEnt)
{
	//if A does not have Circle Collider, or B does not have circle collider, or A and B are the same entity, return false
	if (!cirEnt ->hasComponent<CCircleCollider>() || !boxEnt->hasComponent<CBoxCollider>() || cirEnt == boxEnt) return false;

	return false;
}

bool Physics::collisionBoxToBox(std::shared_ptr<Entity> A, std::shared_ptr<Entity> B)
{
	//getting components
	CBoxCollider& Abox = A->getComponent<CBoxCollider>();
	CBoxCollider& Bbox = B->getComponent<CBoxCollider>();
	CTransform& Atrans = A->getComponent<CTransform>();
	CTransform& Btrans = B->getComponent<CTransform>();

	//if A does not have Circle Collider, or B does not have circle collider, or A and B are the same entity, return false
	if (!Abox.exists || !Bbox.exists || A == B) return false;

	Vector2<float> positionDelta = { abs(Atrans.position.x - Btrans.position.x), abs(Atrans.position.y - Btrans.position.y) };

	if ((Abox.halfSize.x + Bbox.halfSize.x) < positionDelta.x) return false;
	if ((Abox.halfSize.y + Bbox.halfSize.y) < positionDelta.y) return false;

	return true;
}

bool Physics::isColliding(std::shared_ptr<Entity> A, std::shared_ptr<Entity> B)
{
	return false;
}
