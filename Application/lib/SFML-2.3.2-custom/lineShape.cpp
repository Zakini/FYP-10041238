#include "lineShape.h"

#include "../../src/vectorMaths.h"

using jw::maths::normalise;

namespace jw
{
	lineShape::lineShape(Vector2f line, float thickness, Color p_colour)
		: lineVertices(sf::Quads, 4)
		, _lineVector(line)
		, lineThickness(thickness)
	{
		calcVertices();
		colour(p_colour);
	}

	lineShape::lineShape(Vector2f from, Vector2f to, float thickness, Color p_colour)
		: lineShape(to - from, thickness, p_colour)
	{
		setPosition(from);
	}

	void lineShape::lineVector(Vector2f newVector)
	{
		_lineVector = newVector;
		calcVertices();
	}

	void lineShape::lineVector(Vector2f from, Vector2f to)
	{
		lineVector(to - from);
		setPosition(from);
	}

	void lineShape::thickness(float newThickness)
	{
		lineThickness = newThickness;
		calcVertices();
	}

	void lineShape::colour(Color newColour)
	{
		lineColour = newColour;

		// set colour on vertices
		for (unsigned int i = 0; i < lineVertices.getVertexCount(); i++)
		{
			lineVertices[i].color = lineColour;
		}
	}

	void lineShape::draw(RenderTarget& target, RenderStates states) const
	{
		states.transform *= getTransform();
		target.draw(lineVertices, states);
	}

	void lineShape::calcVertices()
	{
		Vector2f unitLineVector = normalise(_lineVector);
		Vector2f perpendicular(-unitLineVector.y, unitLineVector.x);
		Vector2f centreOffset = (lineThickness / 2.0f) * perpendicular;

		lineVertices[0].position = Vector2f(0, 0) + centreOffset;
		lineVertices[1].position = _lineVector + centreOffset;
		lineVertices[2].position = _lineVector - centreOffset;
		lineVertices[3].position = Vector2f(0, 0) - centreOffset;
	}
}