#include "stdafx.h"
#include "CppUnitTest.h"

#include "../../Application/src/collidable.h"
#include "../../Application/src/vectorMaths.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	struct testCollidable : public jw::collidable
	{
		virtual sf::Vector2f getPosition() override { return sf::Vector2f(boundingBox.left + boundingBox.width / 2, boundingBox.top + boundingBox.height / 2); }
		virtual sf::FloatRect getBoundingBox() override { return boundingBox; }
		virtual sf::Vector2f getVelocity() override { return velocity; }
		virtual sf::Vector2f getHeading() override { return jw::maths::normalise(velocity); }

		sf::FloatRect boundingBox;
		sf::Vector2f velocity;
	};

	TEST_CLASS(collidable)
	{
		TEST_METHOD(collideNoCollision)
		{
			testCollidable testObj1 = {}, testObj2 = {};
			testObj1.boundingBox = sf::FloatRect(0, 0, 2, 2);
			testObj2.boundingBox = sf::FloatRect(0, 3, 2, 2);

			Assert::IsFalse(testObj2.collide(testObj1));
		}

		TEST_METHOD(collideCollision)
		{
			testCollidable testObj1 = {}, testObj2 = {};
			testObj1.boundingBox = sf::FloatRect(0, 0, 2, 2);
			testObj2.boundingBox = sf::FloatRect(0, 1, 2, 2);

			Assert::IsTrue(testObj2.collide(testObj1));
		}

		TEST_METHOD(collideAlmostCollision)
		{
			testCollidable testObj1 = {}, testObj2 = {};
			testObj1.boundingBox = sf::FloatRect(0, 0, 2, 2);
			testObj2.boundingBox = sf::FloatRect(2.1f, 2.1f, 2, 2);

			Assert::IsFalse(testObj2.collide(testObj1));
		}

		TEST_METHOD(collideBarelyCollision)
		{
			testCollidable testObj1 = {}, testObj2 = {};
			testObj1.boundingBox = sf::FloatRect(0, 0, 2, 2);
			testObj2.boundingBox = sf::FloatRect(2, 2, 2, 2);

			Assert::IsTrue(testObj2.collide(testObj1));
		}

		TEST_METHOD(collideSelf)
		{
			testCollidable testObj1 = {};

			Assert::IsFalse(testObj1.collide(testObj1));
		}

		TEST_METHOD(sweepCollideNoMovement)
		{
			testCollidable testObj1 = {}, testObj2 = {};

			testObj1.boundingBox = sf::FloatRect(1, 1, 2, 2);
			testObj1.velocity = sf::Vector2f(0, 0);
			testObj2.boundingBox = sf::FloatRect(10, 10, 2, 2);
			testObj2.velocity = sf::Vector2f(0, 0);

			auto collisionResult = testObj2.sweepCollide(testObj1, sf::seconds(10));

			Assert::IsFalse(collisionResult.first);
		}

		TEST_METHOD(sweepCollideNoCollision)
		{
			testCollidable testObj1 = {}, testObj2 = {};

			testObj1.boundingBox = sf::FloatRect(1, 1, 2, 2);
			testObj1.velocity = sf::Vector2f(1, 1);
			testObj2.boundingBox = sf::FloatRect(10, 10, 2, 2);
			testObj2.velocity = sf::Vector2f(1, 1);

			auto collisionResult = testObj2.sweepCollide(testObj1, sf::seconds(10));

			Assert::IsFalse(collisionResult.first);
		}

		TEST_METHOD(sweepCollideAlmostNoCollision)
		{
			testCollidable testObj1 = {}, testObj2 = {};

			testObj1.boundingBox = sf::FloatRect(1, 0, 2, 2);
			testObj1.velocity = sf::Vector2f(2, 0);
			testObj2.boundingBox = sf::FloatRect(4, 0, 2, 2);
			testObj2.velocity = sf::Vector2f(1, 0);

			auto collisionResult = testObj2.sweepCollide(testObj1, sf::seconds(0.9f));

			Assert::IsFalse(collisionResult.first);
		}

		TEST_METHOD(sweepCollideBarelyCollision)
		{
			testCollidable testObj1 = {}, testObj2 = {};

			testObj1.boundingBox = sf::FloatRect(1, 0, 2, 2);
			testObj1.velocity = sf::Vector2f(2, 0);
			testObj2.boundingBox = sf::FloatRect(4, 0, 2, 2);
			testObj2.velocity = sf::Vector2f(1, 0);

			auto collisionResult = testObj2.sweepCollide(testObj1, sf::seconds(1));

			Assert::IsTrue(collisionResult.first);
			Assert::IsTrue(collisionResult.second == sf::Vector2f(4, 1));
		}

		TEST_METHOD(sweepCollideCollision)
		{
			testCollidable testObj1 = {}, testObj2 = {};

			testObj1.boundingBox = sf::FloatRect(0, 1, 2, 2);
			testObj1.velocity = sf::Vector2f(0, 2);
			testObj2.boundingBox = sf::FloatRect(0, 4, 2, 2);
			testObj2.velocity = sf::Vector2f(0, 1);

			auto collisionResult = testObj2.sweepCollide(testObj1, sf::seconds(5));

			Assert::IsTrue(collisionResult.first);
			Assert::IsTrue(collisionResult.second == sf::Vector2f(1, 4));
		}

		TEST_METHOD(sweepCollideCollisionWithMinDistance)
		{
			testCollidable testObj1 = {}, testObj2 = {};

			testObj1.boundingBox = sf::FloatRect(0, 1, 2, 2);
			testObj1.velocity = sf::Vector2f(0, 2);
			testObj2.boundingBox = sf::FloatRect(0, 6, 2, 2);
			testObj2.velocity = sf::Vector2f(0, 1);

			auto collisionResult = testObj2.sweepCollide(testObj1, sf::seconds(2), 2);

			Assert::IsTrue(collisionResult.first);
			Assert::IsTrue(collisionResult.second == sf::Vector2f(1, 4));
		}

		TEST_METHOD(sweepCollideDiagonalMiss)
		{
			testCollidable testObj1 = {}, testObj2 = {};

			testObj1.boundingBox = sf::FloatRect(0, 2, 2, 2);
			testObj1.velocity = sf::Vector2f(1, -1);
			testObj2.boundingBox = sf::FloatRect(5, 3, 2, 2);
			testObj2.velocity = sf::Vector2f(-1, 1);

			auto collisionResult = testObj2.sweepCollide(testObj1, sf::seconds(3));

			Assert::IsFalse(collisionResult.first);
		}

		TEST_METHOD(sweepCollideDiagonalCollision)
		{
			testCollidable testObj1 = {}, testObj2 = {};

			testObj1.boundingBox = sf::FloatRect(0, 0, 2, 2);
			testObj1.velocity = sf::Vector2f(1, 1);
			testObj2.boundingBox = sf::FloatRect(4, 4, 2, 2);
			testObj2.velocity = sf::Vector2f(-1, -1);

			auto collisionResult = testObj2.sweepCollide(testObj1, sf::seconds(3));

			Assert::IsTrue(collisionResult.first);
			Assert::IsTrue(collisionResult.second == sf::Vector2f(2, 2));
		}

		TEST_METHOD(sweepCollideSelf)
		{
			testCollidable testObj1 = {};

			auto collisionResult = testObj1.sweepCollide(testObj1, sf::seconds(10));

			Assert::IsFalse(collisionResult.first);
		}

		TEST_METHOD(minkowskiDifference)
		{
			sf::FloatRect rectA(5, -4, 2, 5);
			sf::FloatRect rectB(-3, 3, 8, 10);
			sf::FloatRect expectedRect(rectA.left - (rectB.left + rectB.width), rectA.top - (rectB.top + rectB.height), rectA.width + rectB.width, rectA.height + rectB.height);

			Assert::IsTrue(expectedRect == jw::collidable::minkowskiDifference(rectA, rectB));
		}

		TEST_METHOD(lineRectIntZeroMiss)
		{
			float intersection = jw::collidable::lineRectIntersection(sf::FloatRect(1, 1, 2, 2), sf::Vector2f(-1, 1), sf::Vector2f(1, 0));
			Assert::IsTrue(intersection == std::numeric_limits<float>::infinity());
		}

		TEST_METHOD(lineRectIntZeroEndsEarly)
		{
			float intersection = jw::collidable::lineRectIntersection(sf::FloatRect(2, 2, 2, 2), sf::Vector2f(1, 1));
			Assert::IsTrue(intersection == std::numeric_limits<float>::infinity());
		}

		TEST_METHOD(lineRectIntOne)
		{
			float intersection = jw::collidable::lineRectIntersection(sf::FloatRect(1, 1, 2, 2), sf::Vector2f(0, 2), sf::Vector2f(1, 0));
			Assert::IsTrue(intersection == 0.5f);
		}

		TEST_METHOD(lineRectIntTwo)
		{
			float intersection = jw::collidable::lineRectIntersection(sf::FloatRect(1, 1, 2, 2), sf::Vector2f(4, 4), sf::Vector2f(0, 1));
			Assert::IsTrue(intersection == 0.25f);
		}

		TEST_METHOD(lineRectIntTwoCorner)
		{
			float intersection = jw::collidable::lineRectIntersection(sf::FloatRect(1, 1, 2, 2), sf::Vector2f(2, 2));
			Assert::IsTrue(intersection == 0.5f);
		}

		TEST_METHOD(lineRectIntThree)
		{
			float intersection = jw::collidable::lineRectIntersection(sf::FloatRect(1, 1, 2, 2), sf::Vector2f(0, 4), sf::Vector2f(1, 0));
			Assert::IsTrue(intersection == 0.25f);
		}

		TEST_METHOD(lineLineInt)
		{
			float intersection = jw::collidable::lineLineIntersection(sf::Vector2f(0, 0), sf::Vector2f(2, 2), sf::Vector2f(2, 0), sf::Vector2f(0, 2));
			Assert::IsTrue(intersection == 0.5f);

			intersection = jw::collidable::lineLineIntersection(sf::Vector2f(0, 0), sf::Vector2f(4, 4), sf::Vector2f(2, 0), sf::Vector2f(0, 2));
			Assert::IsTrue(intersection == 0.25f);
		}

		TEST_METHOD(lineLineIntEndToMiddle)
		{
			float intersection = jw::collidable::lineLineIntersection(sf::Vector2f(0, 0), sf::Vector2f(2, 2), sf::Vector2f(1, 1), sf::Vector2f(0, 2));
			Assert::IsTrue(intersection == 0.5f);
		}

		TEST_METHOD(lineLineIntAtEnd)
		{
			float intersection = jw::collidable::lineLineIntersection(sf::Vector2f(0, 0), sf::Vector2f(2, 2), sf::Vector2f(2, 4), sf::Vector2f(2, 2));
			Assert::IsTrue(intersection == 1);

			intersection = jw::collidable::lineLineIntersection(sf::Vector2f(0, 0), sf::Vector2f(2, 2), sf::Vector2f(2, 0), sf::Vector2f(0, 0));
			Assert::IsTrue(intersection == 0);
		}

		TEST_METHOD(lineLineIntMiss)
		{
			float intersection = jw::collidable::lineLineIntersection(sf::Vector2f(0, 0), sf::Vector2f(1, 0), sf::Vector2f(2, 2), sf::Vector2f(2, 1));
			Assert::IsTrue(intersection == std::numeric_limits<float>::infinity());
		}

		TEST_METHOD(lineLineIntParallel)
		{
			float intersection = jw::collidable::lineLineIntersection(sf::Vector2f(0, 0), sf::Vector2f(10, 0), sf::Vector2f(0, 1), sf::Vector2f(10, 1));
			Assert::IsTrue(intersection == std::numeric_limits<float>::infinity());
		}

		TEST_METHOD(lineLineIntColinearNoOverlap)
		{
			float intersection = jw::collidable::lineLineIntersection(sf::Vector2f(0, 0), sf::Vector2f(2, 2), sf::Vector2f(3, 3), sf::Vector2f(5, 5));
			Assert::IsTrue(intersection == std::numeric_limits<float>::infinity());
		}

		TEST_METHOD(lineLineIntColinearOverlap)
		{
			// end points overlap
			float intersection = jw::collidable::lineLineIntersection(sf::Vector2f(0, 0), sf::Vector2f(1, 1), sf::Vector2f(2, 2), sf::Vector2f(1, 1));
			Assert::IsTrue(intersection == 1);

			// partial overlap
			intersection = jw::collidable::lineLineIntersection(sf::Vector2f(0, 0), sf::Vector2f(2, 2), sf::Vector2f(1, 1), sf::Vector2f(3, 3));
			Assert::IsTrue(intersection == 0.5f);

			// perfect overlap
			intersection = jw::collidable::lineLineIntersection(sf::Vector2f(0, 0), sf::Vector2f(2, 2), sf::Vector2f(2, 2), sf::Vector2f(0, 0));
			Assert::IsTrue(intersection == 0);

			// bigger line completely overlaps smaller line
			intersection = jw::collidable::lineLineIntersection(sf::Vector2f(0, 0), sf::Vector2f(4, 4), sf::Vector2f(1, 1), sf::Vector2f(3, 3));
			Assert::IsTrue(intersection == 0.25f);
		}
	};
}