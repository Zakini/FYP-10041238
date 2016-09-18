#include "road.h"

#include <typeinfo>
#include "location.h"
#include "vectorMaths.h"
#include <utility>

using jw::maths::length;
using jw::maths::normalise;

namespace jw
{
	const float road::defaultRenderDepth = 0.0f;
	const float road::renderOffset = 3.0f;

	road::road(location* p_from, location* p_to)
		: _from(p_from)
		, _to(p_to)
		, fromPosition()
		, toPosition()
		, gameObject(defaultRenderDepth)
	{
		if (_from == nullptr || _to == nullptr) return;

		calcPositions();

		renderShape.lineVector(fromPosition, toPosition);
		renderShape.thickness(4.0f);
	}

	float road::cost()
	{
		return _from->pathingHeuristic(_to);
	}

	void road::flip()
	{
		std::swap(_from, _to);
		calcPositions();
		renderShape.lineVector(fromPosition, toPosition);
	}

	void road::update(sf::Time timeSinceLastFrame)
	{
	}

	void road::draw(sf::RenderWindow& renderTarget)
	{
		renderTarget.draw(renderShape);
	}

	location* road::from()
	{
		return _from;
	}

	location* road::to()
	{
		return _to;
	}

	void road::calcPositions()
	{
		fromPosition = _from->position();
		toPosition = _to->position();

		Vector2f unitLineVector = normalise(toPosition - fromPosition);
		Vector2f perpendicular(-unitLineVector.y, unitLineVector.x);	// points left
		Vector2f leftOffset = perpendicular * renderOffset;

		fromPosition += leftOffset;
		toPosition += leftOffset;
	}
}