#include "Physics.h"

bool Physics::collisionCirToCir(CollisionData& collision)
{
	std::shared_ptr<Entity> A = collision.A;
	std::shared_ptr<Entity> B = collision.B;
	//if A does not have Circle Collider, or B does not have circle collider, or A and B are the same entity, return false
	if (!A->hasComponent<CCircleCollider>() || !B->hasComponent<CCircleCollider>() || A==B) return false;

	//calculate the sum of radius
	float radiusSum = A->getComponent<CCircleCollider>().radius + B->getComponent<CCircleCollider>().radius;

	// normal of the collision vector (vector from A to B)
	Vector2<float> collisionNormal = B->getComponent<CTransform>().position - A->getComponent<CTransform>().position;

	// compare squared radius with squared distance, if sum of radius is less than distance, there is no collision
	if (radiusSum * radiusSum < collisionNormal.magnitudeSqrd())
	{
		return false;
	}

	// so, if there is a collision, now calculate the collision data
	// get the distance from A to B
	float distance = A->getComponent<CTransform>().position.distance(B->getComponent<CTransform>().position);
	if (distance != 0)
	{
		collision.penDist = radiusSum - distance;
		collision.normal = collisionNormal.normalized();
	}
	else
	{
		collision.penDist = A->getComponent<CCircleCollider>().radius;
		collision.normal = Vector2<float>(1.0f, 0.0f);
	}
	return true;
}

bool Physics::collisionCirToBox(CollisionData& collision)
{
	std::shared_ptr<Entity> cirEnt = collision.A;
	std::shared_ptr<Entity> boxEnt = collision.B;
	//if A does not have Circle Collider, or B does not have circle collider, or A and B are the same entity, return false
	if (!cirEnt ->hasComponent<CCircleCollider>() || !boxEnt->hasComponent<CBoxCollider>() || cirEnt == boxEnt) return false;

	return false;
}

bool Physics::collisionBoxToBox(CollisionData& collision)
{
	std::shared_ptr<Entity> A = collision.A;
	std::shared_ptr<Entity> B = collision.B;
	//getting components
	CBoxCollider& Abox = A->getComponent<CBoxCollider>();
	CBoxCollider& Bbox = B->getComponent<CBoxCollider>();
	Vector2<float> Apos = A->getComponent<CTransform>().position;
	Vector2<float> Bpos = B->getComponent<CTransform>().position;

	//if A does not have Circle Collider, or B does not have circle collider, or A and B are the same entity, return false
	if (!Abox.exists || !Bbox.exists || A->getId() == B->getId()) return false;


	Vector2<float> collisionNormal = Bpos - Apos;

	Vector2<float> overlapBox = { ((Abox.halfSize.x + Bbox.halfSize.x) - abs(collisionNormal.x)),
								  ((Abox.halfSize.y + Bbox.halfSize.y) - abs(collisionNormal.y))};

	// if either the "width" or "height" of the overlap box is negative, there is no collision
	if (overlapBox.x < 0 || overlapBox.y < 0) return false;


	//now to resolve the collision, choosing a normal direction for the impact
	//if the overalp is bigger in the y direction
	if (overlapBox.x < overlapBox.y)
	{
		if (collisionNormal.y < 0)
		{
			collision.normal = Vector2<float>(0.0f, -1.0f);
		}
		else
		{
			collision.normal = Vector2<float>(0.0f, 1.0f);
		}
		collision.penDist = overlapBox.y;
	}
	else
	{
		if (collisionNormal.x < 0)
		{
			collision.normal = Vector2<float>(-1.0f, 0.0f);
		}
		else
		{
			collision.normal = Vector2<float>(0.0f, 0.0f);
		}
		collision.penDist = overlapBox.x;
	}
	return true;
}

bool Physics::isColliding(CollisionData& collision)
{
	return false;
}

void Physics::resolveCollision(CollisionData& collision)
{
	std::shared_ptr<Entity> A = collision.A;
	std::shared_ptr<Entity> B = collision.B;
	CRigidbody& ARb = A->getComponent<CRigidbody>();
	CRigidbody& BRb = B->getComponent<CRigidbody>();

	Vector2<float> relativeVel = BRb.velocity - ARb.velocity;

	//relative velocity in the direction of the collision normal
	float speedOnNormal = relativeVel.dot(collision.normal);

	// meaning, if objects are moving away from each other (so there is no force applied on the collision normal) return;
	if (speedOnNormal >= 0) return;
	std::cout << "Applying force for collission between entity ID : " << collision.A->getId() << " and ID : " << collision.B->getId() << std::endl;
	//get the smaller bounceCoef of the two Rb
	float bounceCoefMin = std::min(ARb.bounceCoef, BRb.bounceCoef);

	//speed on collision normal modified by bounce coef
	float impactMagnitude = -(bounceCoefMin + 1.0f) * speedOnNormal;
	impactMagnitude /= (ARb.inv_mass + BRb.inv_mass);

	Vector2<float> impulseVec = collision.normal * impactMagnitude;

	//distribute the magnitde of the impact according to masses
	float massSum = ARb.mass + BRb.mass;
	float aRatio = ARb.mass / massSum;
	float bRatio = BRb.mass / massSum;

	//apply the proportional impulse vector to each RB
	ARb.velocity -= impulseVec * aRatio;
	BRb.velocity += impulseVec * bRatio;
}

void Physics::bruteForceCorrection(CollisionData& collision)
{
	//correction strength
	const float percent = 0.1f;

	//penetratoin threshold to correct, form 0.01 to 0.1
	const float correctionThreshold = 0.01f;

	float correctionMagnitude = std::max(collision.penDist - correctionThreshold, 0.0f) / 
		(collision.A->getComponent<CRigidbody>().inv_mass + collision.B->getComponent<CRigidbody>().inv_mass);

	Vector2<float> correction = collision.normal * percent * correctionMagnitude;

	collision.A->getComponent<CTransform>().position -= correction * collision.A->getComponent<CRigidbody>().inv_mass;
	collision.B->getComponent<CTransform>().position += correction * collision.B->getComponent<CRigidbody>().inv_mass;
}
