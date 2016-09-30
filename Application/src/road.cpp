#include "road.h"

#include <typeinfo>
#include "location.h"
#include "vectorMaths.h"
#include <utility>

using jw::maths::length;
using jw::maths::normalise;
using jw::maths::circleLineFirstIntersection;
using jw::maths::leftPerpendicular;

namespace jw
{
	const float road::defaultRenderDepth = 0.0f;
	const float road::renderOffset = 3.0f;

	road::road(location* p_from, location* p_to)
		: _from(p_from)
		, _to(p_to)
		, _startPosition()
		, _endPosition()
		, gameObject(defaultRenderDepth)
	{
		if (_from == nullptr || _to == nullptr) return;

		calcPositions();

		renderShape.lineVector(_startPosition, _endPosition);
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
		renderShape.lineVector(_startPosition, _endPosition);
	}

	void road::update(sf::Time timeSinceLastFrame)
	{
	}

	void road::draw(RenderTarget& target, RenderStates states) const
	{
		target.draw(renderShape);
	}

	void road::calcPositions()
	{
		// get line between locations
		Vector2f startLocation = _from->position();
		Vector2f endLocation = _to->position();

		// offset
		Vector2f unitLineVector = normalise(endLocation - startLocation);
		Vector2f perpendicular = leftPerpendicular(unitLineVector);
		Vector2f leftOffset = perpendicular * renderOffset;

		Vector2f tempStartPosition = startLocation + leftOffset;
		Vector2f tempEndPosition = endLocation + leftOffset;

		// shorten
		_startPosition = circleLineFirstIntersection(tempEndPosition, tempStartPosition, startLocation, _from->renderRadius());
		_endPosition = circleLineFirstIntersection(tempStartPosition, tempEndPosition, endLocation, _to->renderRadius());
	}
}