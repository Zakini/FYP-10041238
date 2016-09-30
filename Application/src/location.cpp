#include "location.h"

#include <typeinfo>
#include "vectorMaths.h"

using jw::maths::length;
using jw::maths::normalise;
using jw::maths::leftPerpendicular;

const float defaultRenderDepth = 1.0f;

namespace jw
{
	const float location::lightRenderOffset = 2.0f;

	location::location(sf::Vector2f p_position, junctionController::behaviour junctionType, float shapeSize)
		: _position(p_position)
		, renderShape(shapeSize)
		, trafficLightShapes()
		, inboundRoads()
		, controller()
		, gameObject(defaultRenderDepth)
	{
		renderShape.setOrigin(renderShape.getRadius(), renderShape.getRadius());
		renderShape.setFillColor(sf::Color::White);

		if (junctionType != junctionController::behaviour::none)
		{
			controller = new junctionController(junctionType);
		}
	}

	sf::Vector2f location::position()
	{
		return _position;
	}

	void location::position(sf::Vector2f newPosition)
	{
		_position = newPosition;
	}

	void location::addRoad(road* newRoad)
	{
		inboundRoads.push_back(newRoad);

		if (controller != nullptr)
		{
			sf::CircleShape trafficLight(3.0f);
			trafficLight.setOrigin(trafficLight.getRadius(), trafficLight.getRadius());

			sf::Vector2f lightPosition = newRoad->endPosition();
			sf::Vector2f roadUnitVector = normalise(newRoad->endPosition() - newRoad->startPosition());
			sf::Vector2f roadLeftPerpendicular = leftPerpendicular(roadUnitVector);
			trafficLight.setPosition(lightPosition + roadLeftPerpendicular * lightRenderOffset);

			trafficLightShapes.push_back(trafficLight);

			controller->setJunctionCount(inboundRoads.size());
		}
	}

	float location::pathingHeuristic(location* target)
	{
		return length(target->_position - _position);
	}

	void location::update(sf::Time timeSinceLastFrame)
	{
		renderShape.setPosition(_position);

		if (controller != nullptr)
		{
			controller->update(timeSinceLastFrame);

			auto trafficLightState = controller->getState();

			for (auto lightState : trafficLightState)
			{
				switch (lightState.second)
				{
				case junctionController::signalState::stop:
					trafficLightShapes[lightState.first].setFillColor(sf::Color::Red);
					break;

				case junctionController::signalState::prepareToGo:
				case junctionController::signalState::stopIfAble:
					trafficLightShapes[lightState.first].setFillColor(sf::Color::Yellow);
					break;

				case junctionController::signalState::go:
					trafficLightShapes[lightState.first].setFillColor(sf::Color::Green);
					break;
				}
			}
		}
	}

	void location::draw(RenderTarget& target, RenderStates states) const
	{
		target.draw(renderShape);

		for (sf::CircleShape lightShape : trafficLightShapes)
		{
			target.draw(lightShape);
		}
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