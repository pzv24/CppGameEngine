#include "SInput.h"

//hard coding this now to manage the player's ribigbody component
// TODO: make this generic and add a layer of action mapping
void SInput::update(EntityManager& entityManager, const sf::Event& event)
{
	CInput& playerInput = entityManager.getEntities(EntityTag::player).front()->getComponent<CInput>();

	if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
	{
		//std::cout << "Key pressed with code = " << static_cast<int>(keyPressed->scancode) << std::endl;
		switch (keyPressed->scancode)
		{
		case sf::Keyboard::Scancode::W: 
			playerInput.up = true; break;
		case sf::Keyboard::Scancode::A:
			playerInput.left = true; break;;
		case sf::Keyboard::Scancode::S:
			playerInput.down = true; break;;
		case sf::Keyboard::Scancode::D:
			playerInput.right = true; break;
		case sf::Keyboard::Scancode::E:
			playerInput.e = true; break;
		default: break;
		}
	}

	if (const auto* keyReleased = event.getIf<sf::Event::KeyReleased>())
	{
		//std::cout << "Key released with code = " << static_cast<int>(keyReleased->scancode) << std::endl;
		switch (keyReleased->scancode)
		{
		case sf::Keyboard::Scancode::W:
			playerInput.up = false; break;;
		case sf::Keyboard::Scancode::A:
			playerInput.left = false; break;
		case sf::Keyboard::Scancode::S:
			playerInput.down = false; break;;
		case sf::Keyboard::Scancode::D:
			playerInput.right = false; break;
		case sf::Keyboard::Scancode::E:
			playerInput.e = false; break;
		default: break;
		}
	}
}

void SInput::updateInputComponent(EntityManager& entityManager, sf::Keyboard::Scancode code)
{
}
