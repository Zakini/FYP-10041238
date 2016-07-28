#include "road.h"

#include <typeinfo>
#include "location.h"
#include "vectorMaths.h"

namespace jw
{
	float road::cost()
	{
		return from->heuristic(to);
	}
}