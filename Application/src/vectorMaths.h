#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <SFML/System/Vector2.hpp>
#include <stdexcept>

namespace jw
{
	namespace maths
	{
		template<typename T>
		T length(sf::Vector2<T> v)
		{
			return sqrt(v.x * v.x + v.y * v.y);
		}

		template<typename T>
		T dotProduct(sf::Vector2<T> v, sf::Vector2<T> w)
		{
			return v.x * w.x + v.y * w.y;
		}

		template<typename T>
		sf::Vector2<T> normalise(sf::Vector2<T> v)
		{
			return v / length(v);
		}

		template<typename T>
		T angleBetween(sf::Vector2<T> v, sf::Vector2<T> w)
		{
			return T(acos(dotProduct(normalise(v), normalise(w))) * 180.0f / M_PI);
		}

		// See: http://stackoverflow.com/a/1088058/5073239
		template<typename T>
		sf::Vector2<T> circleLineFirstIntersection(sf::Vector2<T> lineStart, sf::Vector2<T> lineEnd, sf::Vector2<T> centre, T radius)
		{
			sf::Vector2<T> intersection;

			sf::Vector2<T> unitLineVector = normalise(lineEnd - lineStart);				// D
			T projectionLength = dotProduct(unitLineVector, centre - lineStart);	// t
			sf::Vector2<T> projectionPoint = projectionLength * unitLineVector + lineStart;	// E
			T projectionDistance = length(projectionPoint - centre);

			if (projectionDistance < radius)
			{
				// 2 intersection points (but we only care about the first one)
				T projectionToEdgeDistance = sqrt(radius * radius - projectionDistance * projectionDistance);
				intersection = (projectionLength - projectionToEdgeDistance) * unitLineVector + lineStart;
			}
			else if (projectionDistance == radius)
			{
				// 1 intersection point
				intersection = projectionPoint;
			}
			else
			{
				// line does not intersect circle
				throw std::invalid_argument("line does not intersect circle");
			}

			return intersection;
		}

		template<typename T>
		sf::Vector2<T> leftPerpendicular(sf::Vector2<T> v)
		{
			// TODO this points right, but lets me move roads to the left... what?
			return sf::Vector2<T>(v.y, -v.x);
		}

		// not actually defined in 2D, just returns the magnitude of the new 3D vector
		template<typename T>
		T crossProduct(sf::Vector2<T> a, sf::Vector2<T> b)
		{
			return a.x * b.y - a.y * b.x;
		}
	}
}