#include "collidable.h"

#include <vector>
#include <algorithm>
#include "vectorMaths.h"

namespace jw
{
	// See: https://hamaluik.com/posts/simple-aabb-collision-using-minkowski-difference/
	bool collidable::collide(collidable& target, float minDistance)
	{
		if (&target == this) return false;	// can't collide with yourself!

		sf::FloatRect targetBoundingBox = target.getBoundingBox();
		targetBoundingBox.left -= minDistance;
		targetBoundingBox.top -= minDistance;
		targetBoundingBox.width += minDistance * 2;
		targetBoundingBox.height += minDistance * 2;

		sf::FloatRect minkowskiRect = collidable::minkowskiDifference(this->getBoundingBox(), targetBoundingBox);
		return minkowskiRect.left <= 0 && minkowskiRect.top <= 0 && (minkowskiRect.left + minkowskiRect.width) >= 0 && (minkowskiRect.top + minkowskiRect.height) >= 0;
	}

	// See: https://hamaluik.com/posts/swept-aabb-collision-using-minkowski-difference/
	std::pair<bool, sf::Vector2f> collidable::sweepCollide(collidable& target, sf::Time period, float minDistance)
	{
		std::pair<bool, sf::Vector2f> collisionResult(false, sf::Vector2f());

		if (&target == this) return collisionResult;	// can't collide with yourself!

		sf::FloatRect targetBoundingBox = target.getBoundingBox();
		targetBoundingBox.left -= minDistance;
		targetBoundingBox.top -= minDistance;
		targetBoundingBox.width += minDistance * 2;
		targetBoundingBox.height += minDistance * 2;
		sf::Vector2f targetCentre(targetBoundingBox.left + targetBoundingBox.width / 2, targetBoundingBox.top + targetBoundingBox.height / 2);

		if (this->collide(target, minDistance))	// already colliding
		{
			collisionResult.first = true;
			collisionResult.second = targetCentre;
		}
		else
		{
			sf::FloatRect minkowskiRect = collidable::minkowskiDifference(this->getBoundingBox(), targetBoundingBox);
			sf::Vector2f targetVelocity = target.getVelocity();
			sf::Vector2f relativeTrajectory = (targetVelocity - this->getVelocity()) * period.asSeconds();
			// fraction of relative trajectory until collision
			float collisionFraction = collidable::lineRectIntersection(minkowskiRect, relativeTrajectory);

			if (collisionFraction <= 1)
			{
				collisionResult.first = true;
				collisionResult.second = targetCentre + (targetVelocity * period.asSeconds() * collisionFraction);
			}
		}

		return collisionResult;
	}

	sf::FloatRect collidable::minkowskiDifference(sf::FloatRect a, sf::FloatRect b)
	{
		return sf::FloatRect(a.left - (b.left + b.width), a.top - (b.top + b.height), a.width + b.width, a.height + b.height);
	}

	float collidable::lineRectIntersection(sf::FloatRect rectangle, sf::Vector2f lineDirection, sf::Vector2f lineOrigin)
	{
		std::vector<float> intersections;

		sf::Vector2f topLeft(rectangle.left, rectangle.top);
		sf::Vector2f topRight = topLeft + sf::Vector2f(rectangle.width, 0);
		sf::Vector2f bottomLeft = topLeft + sf::Vector2f(0, rectangle.height);
		sf::Vector2f bottomRight = topLeft + sf::Vector2f(rectangle.width, rectangle.height);

		intersections.push_back(lineLineIntersection(lineOrigin, lineOrigin + lineDirection, topLeft, topRight));
		intersections.push_back(lineLineIntersection(lineOrigin, lineOrigin + lineDirection, topRight, bottomRight));
		intersections.push_back(lineLineIntersection(lineOrigin, lineOrigin + lineDirection, bottomRight, bottomLeft));
		intersections.push_back(lineLineIntersection(lineOrigin, lineOrigin + lineDirection, bottomLeft, topLeft));

		return *std::min_element(intersections.begin(), intersections.end());
	}

	float collidable::lineLineIntersection(sf::Vector2f lineAStart, sf::Vector2f lineAEnd, sf::Vector2f lineBStart, sf::Vector2f lineBEnd)
	{
		float intersection = std::numeric_limits<float>::infinity();

		sf::Vector2f lineADirection = lineAEnd - lineAStart;
		sf::Vector2f lineBDirection = lineBEnd - lineBStart;

		// need to check elements of the formula for edge cases, so we calculate the numerators and denominator separately first
		float denominator = maths::crossProduct(lineADirection, lineBDirection);
		float lineAFractionNumerator = maths::crossProduct(lineBStart - lineAStart, lineBDirection);
		float lineBFractionNumerator = maths::crossProduct(lineBStart - lineAStart, lineADirection);

		// lines are co-linear
		if (lineAFractionNumerator == 0 && denominator == 0)
		{
			float lineALength = maths::length(lineADirection);
			float aStartToBStart = maths::length(lineBStart - lineAStart);
			float aStartToBEnd = maths::length(lineBEnd - lineAStart);
			float smallestAToB = aStartToBStart < aStartToBEnd ? aStartToBStart : aStartToBEnd;

			// lines overlap
			if (smallestAToB <= lineALength)
			//if ((lineBStart.x - lineAStart.x < 0) != (lineBStart.x - lineAEnd.x < 0) != (lineBEnd.x - lineAStart.x < 0) != (lineBEnd.x - lineAEnd.x < 0)
			//	|| (lineBStart.y - lineAStart.y < 0) != (lineBStart.y - lineAEnd.y < 0) != (lineBEnd.y - lineAStart.y < 0) != (lineBEnd.y - lineAEnd.y < 0))
			{
				intersection = smallestAToB / lineALength;
			}
			// else lines don't overlap
		}
		else if (denominator != 0)
		{
			float lineAFraction = lineAFractionNumerator / denominator;
			float lineBFraction = lineBFractionNumerator / denominator;

			// lines intersect
			if (lineAFraction >= 0 && lineAFraction <= 1 && lineBFraction >= 0 && lineBFraction <= 1)
			{
				intersection = lineAFraction;
			}
		}
		// else lines are parallel

		return intersection;
	}
}