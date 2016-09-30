#pragma once

#include "location.h"
#include "gameObject.h"
#include "../lib/SFML-2.3.2-custom/lineShape.h"

namespace jw
{
	class location;

	class road : public gameObject
	{
	public:
		road(location* p_from = nullptr, location* p_to = nullptr);

		float cost();
		void flip();

		// Inherited via gameObject
		virtual void update(sf::Time timeSinceLastFrame) override;	// POSSIBLE UT?

		// Accessors
		sf::Vector2f startPosition() { return _startPosition; }
		sf::Vector2f endPosition() { return _endPosition; }
		// Only added for UTs
		location* from() { return _from; }
		location* to() { return _to; }

	private:
		void calcPositions();

		location* _from, * _to;
		sf::Vector2f _startPosition, _endPosition;
		lineShape renderShape;

		const static float defaultRenderDepth;
		const static float renderOffset;

		// Inherited via gameObject
		virtual void draw(RenderTarget& target, RenderStates states) const override;	// POSSIBLE UT?
	};
}