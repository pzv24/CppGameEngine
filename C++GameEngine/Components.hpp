#pragma once
#include "Vector2.hpp"
#include <SFML/Graphics.hpp>

class Component
{
public:
	bool exists = false;
};

enum ColliderType
{
	circle,
	boundingBox,
};
class CTransform : public Component
{
public:
	Vector2<float>	position = { 0.0f, 0.0f };
	float rotation = 0.0f;
	Vector2<float>	scale = { 1.0f, 1.0f };

	CTransform() = default;
	CTransform(const Vector2<float>& pos, float angle, const Vector2<float>& scale)
		: position(pos), rotation(angle), scale(scale) {}
};

class CRigidbody : public Component
{
public: 
	Vector2<float> velocity = { 0.0f, 0.0f };
	float bounceCoef = 0.0f;
	float mass = 1.0f;

	CRigidbody() = default;
	CRigidbody(const Vector2<float>& vel, float bounce, float mass)
		: velocity(vel), bounceCoef(bounce), mass(mass){}

	float Speed() { return velocity.magnitude(); }
};

//sfml circle component
class CCircle : public Component
{
public:
	sf::CircleShape circle;

	CCircle() = default;
	CCircle(float radius, size_t points, const sf::Color& fill, const sf::Color& outline, float thickness)
		:circle(radius, points)
	{
		circle.setFillColor(fill);
		circle.setOutlineColor(outline);
		circle.setOutlineThickness(thickness);
		circle.setOrigin({ radius, radius });
	}
};



class CCollider : public Component
{
public:
	virtual ColliderType getType() = 0;
};

class CCircleCollider : public CCollider
{
public:
	float radius = 0;

	CCircleCollider() = default;
	CCircleCollider(float r)
		:radius(r){}

	// Inherited via CCollider
	ColliderType getType() { return circle; }
};

class CBoundingBox : public CCollider
{
public:
	Vector2<float> size;
	Vector2<float> halfSize;

	CBoundingBox() = default;
	CBoundingBox(const Vector2<float>& size)
		:size(size), halfSize(size / 2)
	{
	}

	// Inherited via CCollider
	ColliderType getType() { return boundingBox; }
};

class CInput : public Component
{
public:
	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
	bool e = false;
	bool space = false;

	CInput() = default;
};

