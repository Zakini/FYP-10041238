#include "stdafx.h"
#include "CppUnitTest.h"
#include <SFML/System/Vector2.hpp>
#include "../../Application/src/road.h"
#include "../../Application/src/location.h"
#include "../../Application/src/vectorMaths.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(road)
	{
	public:
		TEST_METHOD(cost)
		{
			jw::location from(sf::Vector2f(0, 0));
			jw::location to(sf::Vector2f(3, 4));
			jw::road testRoad(&from, &to);

			Assert::IsTrue(testRoad.cost() == 5);
		}

		TEST_METHOD(depth)
		{
			jw::road testRoad;

			Assert::IsTrue(testRoad.depth() == 0);
		}

		TEST_METHOD(flip)
		{
			sf::Vector2f fromVector(1, 1);
			sf::Vector2f toVector(2, 2);

			jw::location from(fromVector);
			jw::location to(toVector);

			jw::road testRoad(&from, &to);
			testRoad.flip();

			Assert::IsTrue(*testRoad.from() == toVector);
			Assert::IsTrue(*testRoad.to() == fromVector);
		}

		TEST_METHOD(from)
		{
			jw::location fromLocation(sf::Vector2f(1, 1));
			jw::location toLocation(sf::Vector2f(2, 2));
			jw::road testRoad(&fromLocation, &toLocation);

			Assert::IsTrue(testRoad.from() == &fromLocation);
		}

		TEST_METHOD(to)
		{
			jw::location fromLocation(sf::Vector2f(1, 1));
			jw::location toLocation(sf::Vector2f(2, 2));
			jw::road testRoad(&fromLocation, &toLocation);

			Assert::IsTrue(testRoad.to() == &toLocation);
		}
	
		TEST_METHOD(startEndPosition)
		{
			sf::Vector2f fromPosition(0, 0);
			sf::Vector2f toPosition(5, 0);
			jw::location fromLocation(fromPosition);
			jw::location toLocation(toPosition);
			jw::road testRoad(&fromLocation, &toLocation);

			sf::Vector2f roadVector = toPosition - fromPosition;
			sf::Vector2f unitPerpendicular = jw::maths::normalise(jw::maths::leftPerpendicular(roadVector));

			sf::Vector2f offsetStart = fromPosition + unitPerpendicular * 3.0f;
			sf::Vector2f offsetEnd = toPosition + unitPerpendicular * 3.0f;

			sf::Vector2f expectedStart = jw::maths::circleLineFirstIntersection(offsetEnd, offsetStart, fromPosition, fromLocation.renderRadius());
			sf::Vector2f expectedEnd = jw::maths::circleLineFirstIntersection(offsetStart, offsetEnd, toPosition, toLocation.renderRadius());

			Assert::IsTrue(testRoad.startPosition() == expectedStart);
			Assert::IsTrue(testRoad.endPosition() == expectedEnd);
		}
	};
}