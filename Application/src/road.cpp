#include "road.h"

#include <typeinfo>
#include "location.h"
#include "vectorMaths.h"

using jw::maths::length;

namespace jw
{
	const float road::defaultRenderDepth = 0.0f;

	float road::cost()
	{
		return from->pathingHeuristic(to);
	}

	void road::update(sf::Time timeSinceLastFrame)
	{
	}

	void road::draw(sf::RenderWindow& renderTarget)
	{
		// Mostly copied from https://github.com/SFML/SFML/wiki/Source:-Line-segment-with-thickness
		// POSSIBLE Move to a LineShape class?

		sf::Vector2f targetVector = to->position() - from->position();
		sf::Vector2f unitVector = targetVector / length(targetVector);
		sf::Vector2f perpendicularVector(-unitVector.y, unitVector.x);

		sf::Vector2f offset = (lineThickness / 2.0f) * perpendicularVector;

		renderShape[0].position = from->position() + offset;
		renderShape[1].position = to->position() + offset;
		renderShape[2].position = to->position() - offset;
		renderShape[3].position = from->position() - offset;

		for (int i = 0; i < renderShape.getVertexCount(); i++)
		{
			renderShape[i].color = sf::Color::Red;
		}

		renderTarget.draw(renderShape);
	}
}