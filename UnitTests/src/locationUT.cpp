#include "stdafx.h"
#include "CppUnitTest.h"
#include "../../Application/src/location.h"
#include <SFML/System/Vector2.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(location)
	{
	public:
		TEST_METHOD(getSetPosition)
		{
			jw::location testLoc;
			sf::Vector2f testVec(1, 2);

			testLoc.position(testVec);
			Assert::IsTrue(testLoc.position() == testVec);
		}

		TEST_METHOD(pathingHeuristic)
		{
			jw::location testSrc(sf::Vector2f(0, 0));
			jw::location testDest(sf::Vector2f(3, 4));

			Assert::IsTrue(testSrc.pathingHeuristic(&testDest) == 5.0f);
		}

		TEST_METHOD(opEquals)
		{
			jw::location testLoc1(sf::Vector2f(1, 2));
			jw::location testLoc2(sf::Vector2f(1, 2));

			Assert::IsTrue(testLoc1 == testLoc2);
		}

		TEST_METHOD(opLessThan)
		{
			jw::location testLoc1(sf::Vector2f(1, 2));
			jw::location testLoc2(sf::Vector2f(3, 4));

			Assert::IsTrue(testLoc1 < testLoc2);
		}

		TEST_METHOD(depth)
		{
			jw::location testLocation;

			Assert::IsTrue(testLocation.depth() == 1);
		}

		TEST_METHOD(addGetRoad)
		{
			jw::location testLocation;

			Assert::IsTrue(testLocation.getInboundRoads() == std::vector<jw::road*>());

			std::vector<jw::road*> testRoads = { new jw::road(), new jw::road(), new jw::road() };

			for (auto testRoad : testRoads)
			{
				testLocation.addRoad(testRoad);
			}

			Assert::IsTrue(testLocation.getInboundRoads() == testRoads);
		}

		TEST_METHOD(getRenderRadius)
		{
			jw::location testLocation(sf::Vector2f(1, 2), jw::junctionController::behaviour::none, 6.0f);

			Assert::IsTrue(testLocation.renderRadius() == 6.0f);
		}
	};
}