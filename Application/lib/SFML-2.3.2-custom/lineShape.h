#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

using sf::Vector2f;
using sf::VertexArray;
using sf::RenderTarget;
using sf::RenderStates;
using sf::Color;

// POSSIBLE move into SFML
// Mostly copied from https://github.com/SFML/SFML/wiki/Source:-Line-segment-with-thickness
namespace jw
{
	class lineShape : public sf::Drawable, public sf::Transformable
	{
	public:
		lineShape(Vector2f line = {0, 0}, float thickness = 1, Color p_colour = Color::Red);
		lineShape(Vector2f from, Vector2f to, float thickness = 1, Color p_colour = Color::Red);
		
		void lineVector(Vector2f newVector);
		void lineVector(Vector2f from, Vector2f to);
		void thickness(float newThickness);
		void colour(Color newColour);

		// Inherited via Drawable
		virtual void draw(RenderTarget& target, RenderStates states) const override;

	private:
		void calcVertices();

		VertexArray lineVertices;
		Vector2f _lineVector;
		float lineThickness;
		Color lineColour;
	};
}