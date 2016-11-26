#pragma once

#include <vector>
#include <functional>
#include "collidable.h"

namespace jw
{
	class collisionDetector
	{
	public:
		using filter_type = std::function<bool(collidable*)>;

		void addCollisionTarget(collidable* newTarget) { collisionTargets.push_back(newTarget); }
		std::vector<collidable*> getCollisionTargets() { return collisionTargets; }
		std::vector<sf::Vector2f> predictCollisions(collidable& target, sf::Time period, float minDistance = 0.0f, filter_type filterFunction = filter_type());
		bool predictCollisionAtPosition(collidable& target, sf::Vector2f position, float minDistance = 0.0f);

	private:
		std::vector<collidable*> collisionTargets;
	};
}