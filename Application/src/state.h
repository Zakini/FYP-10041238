#pragma once

#include <SFML/System/Time.hpp>

namespace jw
{
	class state
	{
	public:
		virtual ~state() = default;
		virtual state* clone() = 0;

		virtual void update(sf::Time period) = 0;
	};

	class nullState : public state
	{
	public:
		// Inherited via state
		virtual nullState* clone() override { return new nullState(*this); }
		virtual void update(sf::Time period) override {}
	};
}