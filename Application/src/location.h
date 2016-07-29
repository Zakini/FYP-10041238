#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>

namespace jw
{
	class location
	{
	public:
		location();
		location(sf::Vector2f p_position, float shapeSize = 10.0f) : _position(p_position), renderShape(shapeSize) {}

		sf::Vector2f position();
		void position(sf::Vector2f newPosition);

		float heuristic(location* target);

		void update();	// TODO UT?
		void draw(sf::RenderWindow& renderTarget);	// TODO UT?

		friend bool operator==(const location& lhs, const location& rhs);
		friend bool operator<(const location& lhs, const location& rhs);	// Purely for std::map<location, road>. Done lexicographically (x then y).

	private:
		sf::Vector2f _position;	// TODO 3D vector?
		sf::CircleShape renderShape;
	};
}