#include "stdafx.h"
#include "CppUnitTest.h"

#include "../../Application/src/collisionDetector.h"
#include "../../Application/src/collidable.h"
#include "../../Application/src/vectorMaths.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	struct testCollidable : public jw::collidable
	{
		virtual sf::Vector2f getPosition() override { return sf::Vector2f(boundingBox.left + boundingBox.width / 2, boundingBox.top + boundingBox.height / 2); }
		virtual sf::Vector2f getVelocity() override { return velocity; }
		virtual sf::Vector2f getHeading() override { return jw::maths::normalise(velocity); }
		virtual sf::FloatRect getBoundingBox() override { return boundingBox; }

		sf::FloatRect boundingBox;
		sf::Vector2f velocity;
	};

	TEST_CLASS(collisionDetector)
	{
		TEST_METHOD(addGetTargets)
		{
			jw::collisionDetector testDetector;

			Assert::IsTrue(testDetector.getCollisionTargets() == std::vector<jw::collidable*>());

			std::vector<jw::collidable*> expectedResult = { new testCollidable(), new testCollidable() };

			testDetector.addCollisionTarget(expectedResult[0]);
			testDetector.addCollisionTarget(expectedResult[1]);

			Assert::IsTrue(testDetector.getCollisionTargets() == expectedResult);
		}

		TEST_METHOD(predictCollisionsNoTargets)
		{
			jw::collisionDetector testDetector;
			testCollidable testCollider = {};
			testCollider.boundingBox = sf::FloatRect(1, 1, 2, 2);
			testCollider.velocity = sf::Vector2f(1, 1);

			testDetector.addCollisionTarget(&testCollider);

			std::vector<sf::Vector2f> expectedResults;

			Assert::IsTrue(testDetector.predictCollisions(testCollider, sf::seconds(10)) == expectedResults);
		}

		TEST_METHOD(predictNoCollisions)
		{
			jw::collisionDetector testDetector;
			testCollidable testCollider = {}, testCollidee = {};

			testCollider.boundingBox = sf::FloatRect(1, 1, 2, 2);
			testCollider.velocity = sf::Vector2f(-1, -1);
			testCollidee.boundingBox = sf::FloatRect(10, 10, 2, 2);
			testCollidee.velocity = sf::Vector2f(1, 1);

			testDetector.addCollisionTarget(&testCollider);
			testDetector.addCollisionTarget(&testCollidee);

			std::vector<sf::Vector2f> expectedResults;

			Assert::IsTrue(testDetector.predictCollisions(testCollider, sf::seconds(10)) == expectedResults);
		}

		TEST_METHOD(predictOneCollision)
		{
			jw::collisionDetector testDetector;
			testCollidable testCollider = {}, testCollidee = {};

			testCollider.boundingBox = sf::FloatRect(1, 1, 2, 2);
			testCollider.velocity = sf::Vector2f(2, 0);
			testCollidee.boundingBox = sf::FloatRect(5, 1, 2, 2);
			testCollidee.velocity = sf::Vector2f(1, 0);

			testDetector.addCollisionTarget(&testCollider);
			testDetector.addCollisionTarget(&testCollidee);

			std::vector<sf::Vector2f> expectedResults;
			expectedResults.push_back(sf::Vector2f(6, 2));

			Assert::IsTrue(testDetector.predictCollisions(testCollider, sf::seconds(10)) == expectedResults);
		}

		TEST_METHOD(predictTwoCollision)
		{
			jw::collisionDetector testDetector;
			testCollidable testCollider = {}, testCollidee1 = {}, testCollidee2 = {};

			testCollider.boundingBox = sf::FloatRect(1, 1, 2, 2);
			testCollider.velocity = sf::Vector2f(2, 0);
			testCollidee1.boundingBox = sf::FloatRect(5, 0, 2, 2);
			testCollidee1.velocity = sf::Vector2f(1, 0);
			testCollidee2.boundingBox = sf::FloatRect(5, 2, 2, 2);
			testCollidee2.velocity = sf::Vector2f(1, 0);

			testDetector.addCollisionTarget(&testCollider);
			testDetector.addCollisionTarget(&testCollidee1);
			testDetector.addCollisionTarget(&testCollidee2);

			std::vector<sf::Vector2f> expectedResults;
			expectedResults.push_back(sf::Vector2f(6, 2));
			expectedResults.push_back(sf::Vector2f(6, 2));

			Assert::IsTrue(testDetector.predictCollisions(testCollider, sf::seconds(10)) == expectedResults);
		}

		TEST_METHOD(predictOneCollisionWithMinDistance)
		{
			jw::collisionDetector testDetector;
			testCollidable testCollider = {}, testCollidee = {};

			testCollider.boundingBox = sf::FloatRect(1, 1, 2, 2);
			testCollider.velocity = sf::Vector2f(2, 0);
			testCollidee.boundingBox = sf::FloatRect(5, 1, 2, 2);
			testCollidee.velocity = sf::Vector2f(1, 0);

			testDetector.addCollisionTarget(&testCollider);
			testDetector.addCollisionTarget(&testCollidee);

			std::vector<sf::Vector2f> expectedResults;
			expectedResults.push_back(sf::Vector2f(4, 2));

			Assert::IsTrue(testDetector.predictCollisions(testCollider, sf::seconds(10), 1) == expectedResults);
		}

		TEST_METHOD(predictTwoCollisionWithMinDistance)
		{
			jw::collisionDetector testDetector;
			testCollidable testCollider = {}, testCollidee1 = {}, testCollidee2 = {};

			testCollider.boundingBox = sf::FloatRect(1, 1, 2, 2);
			testCollider.velocity = sf::Vector2f(2, 0);
			testCollidee1.boundingBox = sf::FloatRect(5, 0, 2, 2);
			testCollidee1.velocity = sf::Vector2f(1, 0);
			testCollidee2.boundingBox = sf::FloatRect(5, 2, 2, 2);
			testCollidee2.velocity = sf::Vector2f(1, 0);

			testDetector.addCollisionTarget(&testCollider);
			testDetector.addCollisionTarget(&testCollidee1);
			testDetector.addCollisionTarget(&testCollidee2);

			std::vector<sf::Vector2f> expectedResults;
			expectedResults.push_back(sf::Vector2f(4, 2));
			expectedResults.push_back(sf::Vector2f(4, 2));

			Assert::IsTrue(testDetector.predictCollisions(testCollider, sf::seconds(10), 1) == expectedResults);
		}

		TEST_METHOD(predictCollisionsFilter)
		{
			jw::collisionDetector testDetector;
			testCollidable testCollider = {}, testCollidee = {};

			testCollidee.velocity = sf::Vector2f(1, 1);

			testDetector.addCollisionTarget(&testCollider);
			testDetector.addCollisionTarget(&testCollidee);
			
			std::vector<sf::Vector2f> expectedResults;

			auto filter = [&testCollider](jw::collidable* collidee)
			{
				return collidee->getVelocity() != sf::Vector2f(1, 1);
			};

			Assert::IsTrue(testDetector.predictCollisions(testCollider, sf::seconds(1), 0.0f, filter) == expectedResults);
		}
	};
}