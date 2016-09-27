#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include <set>
#include <SFML/Graphics/Drawable.hpp>

using std::set;
using sf::Drawable;
using sf::RenderTarget;
using sf::RenderStates;

namespace jw
{
	class gameObject : public Drawable
	{
	public:
		gameObject(float p_depth) : _depth(p_depth) {}

		virtual void update(sf::Time timeSinceLastFrame) = 0;
		virtual float depth() { return _depth; }

		friend bool operator<(const gameObject& lhs, const gameObject& rhs);

	protected:
		float _depth;	// higher numbers are "closer" to the screen

		// Inherited via Drawable
		virtual void draw(RenderTarget& target, RenderStates states) const override = 0;
	};
}