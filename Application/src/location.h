#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <vector>
#include "gameObject.h"
#include "junctionController.h"
#include "road.h"

namespace jw
{
	class road;

	class location : public gameObject
	{
	public:
		location(sf::Vector2f p_position = {0, 0}, junctionController::behaviour junctionType = junctionController::behaviour::none, float shapeSize = 10.0f);

		sf::Vector2f position();
		void position(sf::Vector2f newPosition);
		void addRoad(road* newRoad);
		float renderRadius() { return renderShape.getRadius(); }
		const std::vector<road*>& getInboundRoads() { return inboundRoads; }
		junctionController::signalState getSignalAtRoad(road* from);
		bool isControlledJunction() { return controller != nullptr; }

		float pathingHeuristic(location* target);

		// Inherited from gameObject
		virtual void update(sf::Time timeSinceLastFrame) override;	// POSSIBLE UT?

		friend bool operator==(const location& lhs, const location& rhs);
		friend bool operator<(const location& lhs, const location& rhs);	// Purely for std::map<location, road>. Done lexicographically (x then y).

	private:
		sf::Vector2f _position;	// POSSIBLE 3D vector?
		sf::CircleShape renderShape;
		std::vector<sf::CircleShape> trafficLightShapes;
		std::vector<road*> inboundRoads;	// TODO store fromIds?
		junctionController* controller;
		static const float lightRenderOffset;

		// Inherited via gameObject
		virtual void draw(RenderTarget& target, RenderStates states) const override;	// POSSIBLE UT?
	};
}