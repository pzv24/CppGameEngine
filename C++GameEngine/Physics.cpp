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
								  ((Abox.halfSize.y + Bbox.halfSize.y) - abs(collisionNormal.y)) };

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

bool Physics::collisionCirToBox(CollisionData& collision)
{
	// just flip the A and B entities to match the expected component order
	std::shared_ptr<Entity> A = collision.A;
	collision.A = collision.B;
	collision.B = A;
	return collisionBoxToCir(collision);
}
bool Physics::collisionBoxToCir(CollisionData& collision)
{
	// note that we consider A to be the box collider and B to be the circle collider
	std::shared_ptr<Entity> A = collision.A;
	std::shared_ptr<Entity> B = collision.B;

	//getting components
	CBoxCollider& Abox = A->getComponent<CBoxCollider>();
	CCircleCollider& Bcirc = B->getComponent<CCircleCollider>();
	Vector2<float> Apos = A->getComponent<CTransform>().position;
	Vector2<float> Bpos = B->getComponent<CTransform>().position;

	//if A does not have Circle Collider, or B does not have circle collider, or A and B are the same entity, return false
	if (!A->hasComponent<CBoxCollider>() || !B->hasComponent<CCircleCollider>() || A == B) return false;

	//from the center of the box to the center of the circle
	Vector2<float> posNormal = Bpos - Apos;

	// in case circle is fully inside box. Will use to invert the collision normal at the end
	bool isInside = false;

	//clamp the normal x and y values to the limit of the bounding box
	//this created a point on the edge of the box collider that will be taken as point of contact for the collision
	//note: this point is in local coordenates to A
	Vector2<float> closestPointOfContact(std::clamp(posNormal.x, -Abox.halfSize.x, Abox.halfSize.x), std::clamp(posNormal.y, -Abox.halfSize.y, Abox.halfSize.y));

	//if the circle is fully inside the box
	if (posNormal == closestPointOfContact)
	{
		//snap closest point of contact to nearest axis
		//if the normal is closer to the edge of the x axis, rather than the y axis
		if (Abox.halfSize.x - std::abs(posNormal.x) < Abox.halfSize.y - std::abs(posNormal.y))
		{
			// if position normal x is negative, snap to negative half size, otherwise snap to positive half size
			// set that half box as the point in x
			closestPointOfContact.x = posNormal.x < 0 ? -Abox.halfSize.x : Abox.halfSize.x;
		}
		else
		{
			// repeat in case of y being closer to edge
			closestPointOfContact.y = posNormal.y < 0 ? -Abox.halfSize.y : Abox.halfSize.y;
		}
		isInside = true;
	}

	//create a new normal from the point of contact to the circle center
	//this is the actual normal that we'll be using for the collision
	Vector2<float> collisionNormal = posNormal - closestPointOfContact;

	//there is a collision if: the collision normal is greater than the circle radius
	// or if we have detected the circle to be inside the box

	if (collisionNormal.magnitudeSqrd() < (Bcirc.radius * Bcirc.radius) || isInside)
	{
		//there is collision!
		//now we get the actual magnitude (not squared) of the collision normal
		collision.penDist = Bcirc.radius - collisionNormal.magnitude();

		// if the circle is inside the box, invert the direction of the normal so that it faces outwards FROM the box
		collision.normal = isInside ? collisionNormal.normalized() * -1 : collisionNormal.normalized();
		return true;
	}
	// if no collision is detected, return  false
	else return false;
}


bool Physics::isColliding(CollisionData& collision)
{
	if (collision.A->getId() == collision.B->getId()) return false;

	if (collision.A->hasComponent<CCircleCollider>())
	{
		if (collision.B->hasComponent<CCircleCollider>())
		{
			return collisionCirToCir(collision);
		}
		else if(collision.B->hasComponent<CBoxCollider>())
		{
			return collisionCirToBox(collision);
		}
	}
	else if (collision.A->hasComponent<CBoxCollider>())
	{
		if (collision.B->hasComponent<CBoxCollider>())
		{
			return collisionBoxToBox(collision);
		}
		else if (collision.B->hasComponent<CCircleCollider>())
		{
			return collisionBoxToCir(collision);
		}
	}

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

	//std::cout << "Applying force for collission between entity ID : " << collision.A->getId() << " and ID : " << collision.B->getId() << std::endl;
	
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
