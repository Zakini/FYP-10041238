#include "stdafx.h"
#include "CppUnitTest.h"
#include "../../Application/src/location.h"
#include <SFML/System/Vector2.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(locationTest)
	{
	public:
		TEST_METHOD(positionTest)
		{
			jw::location testLoc;
			sf::Vector2f testVec(1, 2);

			testLoc.position(testVec);
			Assert::IsTrue(testLoc.position() == testVec);
		}

		TEST_METHOD(heuristicTest)
		{
			jw::location testSrc(sf::Vector2f(0, 0));
			jw::location testDest(sf::Vector2f(3, 4));

			Assert::IsTrue(testSrc.heuristic(&testDest) == 5.0f);
		}

		TEST_METHOD(opEqualsTest)
		{
			jw::location testLoc1(sf::Vector2f(1, 2));
			jw::location testLoc2(sf::Vector2f(1, 2));

			Assert::IsTrue(testLoc1 == testLoc2);
		}

		TEST_METHOD(opLessThanTest)
		{
			jw::location testLoc1(sf::Vector2f(1, 2));
			jw::location testLoc2(sf::Vector2f(3, 4));

			Assert::IsTrue(testLoc1 < testLoc2);
		}
	};
}