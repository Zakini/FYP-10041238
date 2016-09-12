#pragma once

#include <SFML/Graphics/Vertex.hpp>
#include "location.h"
#include "gameObject.h"

namespace jw
{
	class road : public gameObject
	{
	public:
		road(location* p_from = nullptr, location* p_to = nullptr, float p_lineThickness = 4.0f)
			: from(p_from)
			, to(p_to)
			, renderShape(sf::Quads, 4)
			, lineThickness(p_lineThickness)
			, gameObject(defaultRenderDepth)
		{}

		float cost();

		// Inherited via gameObject
		virtual void update(sf::Time timeSinceLastFrame) override;	// POSSIBLE UT?
		virtual void draw(sf::RenderWindow& renderTarget) override;	// POSSIBLE UT?

	private:
		location* from, * to;
		sf::VertexArray renderShape;
		float lineThickness;

		const static float defaultRenderDepth;
	};
}