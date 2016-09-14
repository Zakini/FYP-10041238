#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include <set>

using std::set;

namespace jw
{
	class gameObject
	{
	public:
		gameObject(float p_depth) : _depth(p_depth) {}

		virtual void update(sf::Time timeSinceLastFrame) = 0;
		virtual void draw(sf::RenderWindow& renderTarget) = 0;
		virtual float depth() { return _depth; }

		friend bool operator<(const gameObject& lhs, const gameObject& rhs);

	protected:
		float _depth;	// higher numbers are "closer" to the screen
	};
}