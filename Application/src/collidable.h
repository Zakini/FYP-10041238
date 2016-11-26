#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>
#include "physicsObject.h"

namespace jw
{
	class collidable : public physicsObject
	{
	public:
		virtual bool collide(collidable& target, float minDistance = 0.0f);
		virtual bool collideAtPosition(sf::Vector2f position, collidable& target, float minDistance = 0.0f);
		virtual std::pair<bool, sf::Vector2f> sweepCollide(collidable& target, sf::Time period, float minDistance = 0.0f);
		virtual sf::FloatRect getBoundingBox() = 0;

		static sf::FloatRect minkowskiDifference(sf::FloatRect a, sf::FloatRect b);
		static float lineRectIntersection(sf::FloatRect rectangle, sf::Vector2f lineDirection, sf::Vector2f lineOrigin = { 0, 0 });
		static float lineLineIntersection(sf::Vector2f lineAStart, sf::Vector2f lineAEnd, sf::Vector2f lineBStart, sf::Vector2f lineBEnd);
	};
}