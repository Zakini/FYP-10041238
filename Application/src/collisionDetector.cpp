#include "collisionDetector.h"

namespace jw
{
	std::vector<sf::Vector2f> collisionDetector::predictCollisions(collidable& target, sf::Time period, float minDistance)
	{
		std::vector<sf::Vector2f> collisionResults;

		for (collidable* collidee : collisionTargets)
		{
			auto collisionResult = collidee->sweepCollide(target, period, minDistance);

			if (collisionResult.first)
			{
				collisionResults.push_back(collisionResult.second);
			}
		}

		return collisionResults;
	}
}