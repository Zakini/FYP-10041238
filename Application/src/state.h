#pragma once

#include <SFML/System/Time.hpp>

namespace jw
{
	class state
	{
	public:
		virtual ~state() = default;

		virtual void update(sf::Time period) = 0;
	};

	class nullState : public state
	{
	public:
		// Inherited via state
		virtual void update(sf::Time period) override {}
	};
}