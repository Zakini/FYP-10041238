#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include "gameObject.h"

namespace jw
{
	class location : public gameObject
	{
	public:
		location(sf::Vector2f p_position = {0, 0}, float shapeSize = 10.0f);

		sf::Vector2f position();
		void position(sf::Vector2f newPosition);

		float heuristic(location* target);	// TODO rename to pathingHeuristic?

		// Inherited from gameObject
		virtual void update() override;	// TODO UT?
		virtual void draw(sf::RenderWindow& renderTarget) override;	// TODO UT?

		friend bool operator==(const location& lhs, const location& rhs);
		friend bool operator<(const location& lhs, const location& rhs);	// Purely for std::map<location, road>. Done lexicographically (x then y).

	private:
		sf::Vector2f _position;	// TODO 3D vector?
		sf::CircleShape renderShape;
	};
}