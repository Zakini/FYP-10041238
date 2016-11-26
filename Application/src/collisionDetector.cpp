#include "collisionDetector.h"
#include "vectorMaths.h"

namespace jw
{
	std::vector<sf::Vector2f> collisionDetector::predictCollisions(collidable& target, sf::Time period, float minDistance, collisionDetector::filter_type filterFunction)
	{
		std::vector<sf::Vector2f> collisionResults;
		std::vector<collidable*> filteredTargets(collisionTargets.size());

		if (filterFunction)
		{
			auto filteredTargetsEnd = std::copy_if(collisionTargets.begin(), collisionTargets.end(), filteredTargets.begin(), filterFunction);
			filteredTargets.resize(std::distance(filteredTargets.begin(), filteredTargetsEnd));
		}
		else
		{
			filteredTargets = collisionTargets;
		}

		for (collidable* collidee : filteredTargets)
		{
			auto collisionResult = collidee->sweepCollide(target, period, minDistance);

			if (collisionResult.first)
			{
				collisionResults.push_back(collisionResult.second);
			}
		}

		return collisionResults;
	}

	bool collisionDetector::predictCollisionAtPosition(collidable& target, sf::Vector2f position, float minDistance)
	{
		bool collisionResult = false;

		for (collidable* collidee : collisionTargets)
		{
			collisionResult = collidee->collideAtPosition(position, target, minDistance);

			if (collisionResult) break;
		}

		return collisionResult;
	}
}