#pragma once

#include <math.h>
#include "SFML/System/Vector2.hpp"

namespace jw
{
	// TODO namespace

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
}