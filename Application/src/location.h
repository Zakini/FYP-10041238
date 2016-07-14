#pragma once

#include "SFML/System/Vector2.hpp"

namespace jw
{
	class location
	{
	public:
		location() :  _position() {}
		location(sf::Vector2f p_position) : _position(p_position) {}

		sf::Vector2f position();
		void position(sf::Vector2f newPosition);

		float heuristic(location* target);

		friend bool operator==(const location& lhs, const location& rhs);
		friend bool operator<(const location& lhs, const location& rhs);	// Purely for std::map<location, road>. Done lexicographically (x then y).

	private:
		sf::Vector2f _position;
	};
}