#pragma once

#include <SFML/System/Vector2.hpp>

namespace jw
{
	class physicsObject
	{
	public:
		virtual sf::Vector2f getPosition() = 0;
		virtual sf::Vector2f getVelocity() = 0;
		virtual sf::Vector2f getHeading() = 0;
	};
}