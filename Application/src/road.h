#pragma once

#include "location.h"
#include "gameObject.h"
#include "../lib/SFML-2.3.2-custom/lineShape.h"

namespace jw
{
	class road : public gameObject
	{
	public:
		road(location* p_from = nullptr, location* p_to = nullptr);

		float cost();
		void flip();

		// Inherited via gameObject
		virtual void update(sf::Time timeSinceLastFrame) override;	// POSSIBLE UT?
		virtual void draw(sf::RenderWindow& renderTarget) override;	// POSSIBLE UT?

		// Accessors
		// Only added for UTs
		location* from();
		location* to();

	private:
		void calcPositions();

		location* _from, * _to;
		sf::Vector2f fromPosition, toPosition;
		lineShape renderShape;

		const static float defaultRenderDepth;
		const static float renderOffset;
	};
}