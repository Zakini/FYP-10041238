#pragma once

#include <SFML/Graphics/Vertex.hpp>
#include "location.h"
#include "gameObject.h"

namespace jw
{
	class road : public gameObject
	{
	public:
		road(location* p_from = nullptr, location* p_to = nullptr, float p_lineThickness = 4.0f);

		float cost();

		// Inherited via drawable
		virtual void update(sf::Time timeSinceLastFrame) override;	// POSSIBLE UT?
		virtual void draw(sf::RenderWindow& renderTarget) override;	// POSSIBLE UT?

	private:
		location* from, * to;
		sf::VertexArray renderShape;
		float lineThickness;
	};
}