#pragma once
#include "Vector2.hpp"
#include <SFML/Graphics.hpp>

class Component
{
public:
	bool exists = false;
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
	float inv_mass=1.0f; 

	CRigidbody() = default;
	CRigidbody(const Vector2<float>& vel, float bounce, float mass)
		: velocity(vel), bounceCoef(bounce), mass(mass)
	{
		inv_mass = 1.0f / mass;
	}

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

//sfml circle component
class CRectangle : public Component
{
public:
	sf::RectangleShape rectangle;

	CRectangle() = default;
	CRectangle(const Vector2<float>& size, const sf::Color& fill, const sf::Color& outline, float thickness)
		:rectangle(sf::Vector2<float>({size.x,size.y}))
	{
		rectangle.setFillColor(fill);
		rectangle.setOutlineColor(fill);
		rectangle.setOutlineThickness(thickness);
		rectangle.setOrigin({size.x/2, size.y/2});
	}
};


class CCircleCollider : public Component
{
public:
	float radius = 0;

	CCircleCollider() = default;
	CCircleCollider(float r)
		:radius(r){}
};

class CBoxCollider : public Component
{
public:
	Vector2<float> size = { 0.0f, 0.0f };
	Vector2<float> halfSize = { 0.0f, 0.0f };

	CBoxCollider() = default;
	CBoxCollider(const Vector2<float>& size)
		:size(size), halfSize(size / 2)
	{
	}
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

