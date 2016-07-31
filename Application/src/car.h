#pragma once

#include <SFML/System/Vector2.hpp>
#include "gameObject.h"

namespace jw
{
	class car : public gameObject
	{
	public:
		// Inherited via gameObject
		virtual void update(sf::Time timeSinceLastFrame) override;
		virtual void draw(sf::RenderWindow& renderTarget) override;

	private:
		sf::Vector2f position;
		float mass;
	};
}