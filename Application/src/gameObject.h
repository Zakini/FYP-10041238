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
		virtual void update(sf::Time timeSinceLastFrame) = 0;
		virtual void draw(sf::RenderWindow& renderTarget) = 0;
		virtual float depth() { return _depth; }

		friend bool operator<(const gameObject& lhs, const gameObject& rhs);

	protected:
		float _depth;
	};
}