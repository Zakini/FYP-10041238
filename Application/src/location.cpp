#include "location.h"

#include <typeinfo>
#include "vectorMaths.h"

namespace jw
{
	location::location(sf::Vector2f p_position, float shapeSize) : _position(p_position), renderShape(shapeSize)
	{
		_depth = 0;

		renderShape.setOrigin(renderShape.getRadius(), renderShape.getRadius());
		renderShape.setFillColor(sf::Color::White);
	}

	sf::Vector2f location::position()
	{
		return _position;
	}

	void location::position(sf::Vector2f newPosition)
	{
		_position = newPosition;
	}

	float location::pathingHeuristic(location* target)
	{
		return length(target->_position - _position);
	}

	void location::update(sf::Time timeSinceLastFrame)
	{
		renderShape.setPosition(_position);
	}

	void location::draw(sf::RenderWindow& renderTarget)
	{
		renderTarget.draw(renderShape);
	}

	bool operator==(const location& lhs, const location& rhs)
	{
		return lhs._position == rhs._position;
	}

	bool operator<(const location& lhs, const location& rhs)
	{
		sf::Vector2f lPos = lhs._position;
		sf::Vector2f rPos = rhs._position;

		if (lPos.x < rPos.x) return true;
		else if (lPos.x == rPos.x && lPos.y < rPos.y) return true;
		else return false;
	}
}