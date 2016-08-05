#pragma once

#include <SFML/System/Time.hpp>

namespace jw
{
	class state
	{
	public:
		virtual void update(sf::Time period) = 0;
	};
}