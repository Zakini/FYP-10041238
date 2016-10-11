#pragma once

#include <vector>
#include "collidable.h"

namespace jw
{
	class collisionDetector
	{
	public:
		void addCollisionTarget(collidable* newTarget) { collisionTargets.push_back(newTarget); }
		std::vector<collidable*> getCollisionTargets() { return collisionTargets; }
		std::vector<sf::Vector2f> predictCollisions(collidable& target, sf::Time period, float minDistance = 0.0f);

	private:
		std::vector<collidable*> collisionTargets;
	};
}