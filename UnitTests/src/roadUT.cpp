#include "stdafx.h"
#include "CppUnitTest.h"
#include <SFML/System/Vector2.hpp>
#include "../../Application/src/road.h"
#include "../../Application/src/location.h"

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
	
		TEST_METHOD(startPosition)
		{
			Assert::Fail();
		}

		TEST_METHOD(endPosition)
		{
			Assert::Fail();
		}
	};
}