#pragma once

#include <math.h>
#include "SFML/System/Vector2.hpp"

namespace jw
{
	template<typename T>
	T length(sf::Vector2<T> v)
	{
		return sqrt(v.x * v.x + v.y * v.y);
	}
}