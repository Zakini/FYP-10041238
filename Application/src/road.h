#pragma once

#include "location.h"

namespace jw
{
	class road
	{
	public:
		road() : from(), to() {}
		road(location* p_from, location* p_to) : from(p_from), to(p_to) {}

		float cost();

	private:
		location* from, * to;
	};
}