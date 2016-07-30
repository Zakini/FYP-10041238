#include "gameObject.h"

bool jw::operator<(const gameObject& lhs, const gameObject& rhs)
{
	return lhs._depth < rhs._depth;
}