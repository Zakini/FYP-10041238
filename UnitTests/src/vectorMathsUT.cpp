#include "stdafx.h"
#include "CppUnitTest.h"
#include "../../Application/src/vectorMaths.h"
#include <SFML/System/Vector2.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{		
	TEST_CLASS(vectorMaths)
	{
	public:
		TEST_METHOD(length)
		{
			sf::Vector2f testVector(3, 4);
			Assert::IsTrue(jw::maths::length(testVector) == 5.0f);
		}

		TEST_METHOD(dotProduct)
		{
			sf::Vector2f testVector1, testVector2;

			// right angled
			testVector1 = { 1, 0 };
			testVector2 = { 0, 1 };

			Assert::IsTrue(jw::maths::dotProduct(testVector1, testVector2) == 0);

			// aligned
			testVector1 = { 3, 0 };
			testVector2 = { 2, 0 };

			Assert::IsTrue(jw::maths::dotProduct(testVector1, testVector2) == 6);

			// "random"
			testVector1 = { 39, 73 };
			testVector2 = { 134, -669 };

			Assert::IsTrue(jw::maths::dotProduct(testVector1, testVector2) == -43611);
		}

		TEST_METHOD(normalise)
		{
			sf::Vector2f testVector(-305, -529);
			sf::Vector2f normalVector = jw::maths::normalise(testVector);

			// could check they're pointing the same way too but that requires normalising both vectors... which is kinda pointless
			Assert::IsTrue(round(jw::maths::length(normalVector)) == 1);	// 1 unit long
		}

		TEST_METHOD(angleBetween)
		{
			sf::Vector2f testVector1, testVector2;

			// right angled
			testVector1 = { 4, 0 };
			testVector2 = { 0, 2 };

			Assert::IsTrue(jw::maths::angleBetween(testVector1, testVector2) == 90);

			// aligned
			testVector1 = { 3, 0 };
			testVector2 = { 2, 0 };

			Assert::IsTrue(jw::maths::angleBetween(testVector1, testVector2) == 0);

			// "random"
			testVector1 = { -251, -477 };
			testVector2 = { 325, 257 };

			Assert::IsTrue(round(jw::maths::angleBetween(testVector1, testVector2)) == 156);
		}

		TEST_METHOD(circleLineFirstIntersection)
		{
			Assert::Fail();
		}

		TEST_METHOD(leftPerpendicular)
		{
			Assert::Fail();
		}
	};
}