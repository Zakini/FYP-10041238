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

	void road::draw(sf::RenderWindow& renderTarget)
	{
		renderTarget.draw(renderShape);
	}

	void road::calcPositions()
	{
		_startPosition = _from->position();
		_endPosition = _to->position();

		Vector2f unitLineVector = normalise(_endPosition - _startPosition);
		Vector2f perpendicular(-unitLineVector.y, unitLineVector.x);	// points left
		Vector2f leftOffset = perpendicular * renderOffset;

		_startPosition += leftOffset;
		_endPosition += leftOffset;
	}
}