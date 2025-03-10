#pragma once
#include "EntityManager.h"
#include "SFML/Graphics.hpp";
class SInput
{
public:
	void update(EntityManager& entityManager, const sf::Event& event);
	void updateInputComponent(EntityManager& entityManager, sf::Keyboard::Scancode code);
};

