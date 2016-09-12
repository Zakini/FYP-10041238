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
	};
}