#include "stdafx.h"
#include "CppUnitTest.h"
#include <SFML/System/Vector2.hpp>
#include "../../Application/src/road.h"
#include "../../Application/src/location.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(roadTest)
	{
	public:
		TEST_METHOD(costTest)
		{
			jw::location from(sf::Vector2f(0, 0));
			jw::location to(sf::Vector2f(3, 4));
			jw::road testRoad(&from, &to);

			Assert::IsTrue(testRoad.cost() == 5);
		}
	};
}