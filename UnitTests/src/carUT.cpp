#include "stdafx.h"
#include "CppUnitTest.h"

#include "../../Application/src/car.h"
#include <vector>
#include <string>
#include "../../Application/src/world.h"
#include "../../Application/src/pathEngine.h"
#include <fstream>
#include "../../Application/lib/json-master/src/json.hpp"
#include <stdexcept>
#include "../../Application/src/collisionDetector.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(car)
	{
	private:
		const string worldRootPath = "C:/Users/Josh Wells/Google Drive/Uni/Level 6/Final Year Project/Artefact/data/maps/tests";
		const string carRootPath = "C:/Users/Josh Wells/Google Drive/Uni/Level 6/Final Year Project/Artefact/data/entities/tests";

	public:
		TEST_METHOD(loadFromString)	// Tests homeLocation and workLocation methods too
		{
			jw::world::graph_type* testGraph = jw::world::loadWorld(worldRootPath + "/car-unit-test1.json");
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);
			auto testPather = std::make_shared<jw::pathEngine>(testGraphSp);
			auto testDetector = std::make_shared<jw::collisionDetector>();
			std::vector<jw::car*> testCars = jw::car::loadCars(carRootPath + "/load-cars-unit-test1.json", testPather, testDetector);

			// Assert car 0's home is 1
			Assert::IsTrue(testCars[0]->homeLocation() == 1);
			// Assert car 0's work is 2
			Assert::IsTrue(testCars[0]->workLocation() == 2);
			// Assert car 1's home is 3
			Assert::IsTrue(testCars[1]->homeLocation() == 3);
			// Assert car 1's work is 4
			Assert::IsTrue(testCars[1]->workLocation() == 4);
		}

		TEST_METHOD(loadFromJson)	// Tests homeLocation and workLocation methods too
		{
			std::ifstream testFile(carRootPath + "/load-cars-unit-test1.json");
			jw::world::graph_type* testGraph = jw::world::loadWorld(worldRootPath + "/car-unit-test1.json");
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);
			auto testPather = std::make_shared<jw::pathEngine>(testGraphSp);
			auto testDetector = std::make_shared<jw::collisionDetector>();
			nlohmann::json testJson;

			testFile >> testJson;

			std::vector<jw::car*> testCars = jw::car::loadCars(testJson, testPather, testDetector);

			// Assert car 0's home is 1
			Assert::IsTrue(testCars[0]->homeLocation() == 1);
			// Assert car 0's work is 2
			Assert::IsTrue(testCars[0]->workLocation() == 2);
			// Assert car 1's home is 3
			Assert::IsTrue(testCars[1]->homeLocation() == 3);
			// Assert car 1's work is 4
			Assert::IsTrue(testCars[1]->workLocation() == 4);
		}
	
		TEST_METHOD(getHomeLocationId)
		{
			jw::car testCar(nullptr, nullptr, 1, 2);	// nullptr is invalid but works for this test

			Assert::IsTrue(testCar.homeLocation() == 1);
		}

		TEST_METHOD(getWorkLocationId)
		{
			jw::car testCar(nullptr, nullptr, 1, 2);	// nullptr is invalid but works for this test

			Assert::IsTrue(testCar.workLocation() == 2);
		}

		TEST_METHOD(getSetPosition)
		{
			jw::car testCar(nullptr, nullptr, 1, 2);
			sf::Vector2f targetPosition(3, 4);
			testCar.setPosition(targetPosition);

			Assert::IsTrue(testCar.getPosition() == targetPosition);
			Assert::IsTrue(testCar.getVelocity() == sf::Vector2f());
			Assert::IsTrue(testCar.getCurrentSituation() == jw::car::situation::none);
			Assert::IsTrue(testCar.currentPath() == deque<int>());
			Assert::IsTrue(testCar.getTrafficLightPerception() == pair<const Vector2f*, const jw::junctionController::signalState*>(nullptr, nullptr));
			// TODO assert car ahead position removed
		}

		TEST_METHOD(getVelocity)
		{
			jw::car testCar(nullptr, nullptr, 1, 2, jw::fsm());
			sf::Vector2f targetPosition(10000, 0);
			testCar.targetPosition(targetPosition);

			sf::Time updateLength = sf::milliseconds(10);
			sf::Vector2f expectedDirection = jw::maths::normalise(targetPosition);
			sf::Vector2f expectedForce = testCar.getMaxEngineForce() * expectedDirection;	// no friction since car is not moving yet
			sf::Vector2f expectedAcceleration = expectedForce / testCar.getMass();
			sf::Vector2f expectedVelocity = expectedAcceleration * updateLength.asSeconds();

			testCar.moveTowardTarget(updateLength);

			Assert::IsTrue(testCar.getVelocity() == expectedVelocity);
		}

		TEST_METHOD(getHeading)
		{
			jw::car testCar(nullptr, nullptr, 1, 2, jw::fsm());
			sf::Vector2f targetPosition(10000, 0);
			testCar.targetPosition(targetPosition);

			testCar.moveTowardTarget(sf::seconds(1));

			Assert::IsTrue(testCar.getHeading() == sf::Vector2f(1, 0));

			testCar.targetPosition(testCar.getPosition());

			sf::Clock timer;
			while (jw::maths::length(testCar.getVelocity()) != 0)
			{
				testCar.moveTowardTarget(sf::milliseconds(10));
				if (timer.getElapsedTime().asSeconds() > 10) Assert::Fail();	// setup has failed, not getHeading
			}

			Assert::IsTrue(testCar.getHeading() == sf::Vector2f(1, 0));

			jw::world::graph_type* testGraph = jw::world::loadWorld(worldRootPath + "/car-unit-test3.json");
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);

			auto testPather = std::make_shared<jw::pathEngine>(testGraphSp);

			jw::car testCar2(testPather, nullptr, 1, 2, jw::fsm());
			testCar2.currentLocation(1);
			testCar2.pathTo(4);
			testCar2.targetPosition(testPather->getRoadStartPosition(1, 4));

			timer.restart();
			while (!testCar.isAtTarget())
			{
				testCar.moveTowardTarget(sf::milliseconds(10));
				if (timer.getElapsedTime().asSeconds() > 10) Assert::Fail();	// setup has failed, not getHeading
			}

			testCar2.targetPosition(testPather->getRoadEndPosition(1, 4));
			testCar2.moveTowardTarget(sf::milliseconds(10));
			testCar2.update(sf::milliseconds(10));

			Assert::IsTrue(testCar2.getCurrentSituation() == jw::car::situation::onRoad);
			Assert::IsTrue(testCar2.getHeading() == testPather->getRoadDirection(1, 4));

			timer.restart();
			while (!testCar2.isAtTarget())
			{
				testCar2.moveTowardTarget(sf::milliseconds(10));
				if (timer.getElapsedTime().asSeconds() > 10) Assert::Fail();	// setup has failed, not getHeading
			}

			testCar2.moveTowardTarget(sf::milliseconds(10));
			testCar2.update(sf::milliseconds(10));

			Assert::IsTrue(testCar2.getCurrentSituation() == jw::car::situation::atLight);
			Assert::IsTrue(testCar2.getHeading() == testPather->getRoadDirection(1, 4));
		}

		TEST_METHOD(getSetCurrentLocationId)
		{
			jw::world::graph_type* testGraph = jw::world::loadWorld(worldRootPath + "/car-unit-test2.json");
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);

			auto testPather = std::make_shared<jw::pathEngine>(testGraphSp);

			jw::car testCar(testPather, nullptr, 1, 2);

			testCar.currentLocation(2);

			Assert::IsTrue(testCar.currentLocation() == 2);
		}
	
		TEST_METHOD(pathToUpdateAndGetPath)
		{
			jw::world::graph_type* testGraph = jw::world::loadWorld(worldRootPath + "/car-unit-test2.json");
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);

			auto testPather = std::make_shared<jw::pathEngine>(testGraphSp);

			jw::car testCar(testPather, nullptr, 1, 2, jw::fsm());

			testCar.currentLocation(1);
			testCar.pathTo(2);
			testCar.update(sf::seconds(1));

			deque<int> expectedPath1, expectedPath2;
			expectedPath1.push_back(2);

			auto trafficLightPerceptions = testCar.getTrafficLightPerception();
			Assert::IsTrue(trafficLightPerceptions.first != nullptr);

			Assert::IsTrue(testCar.currentPath() == expectedPath1);
			testCar.completePathStep();
			Assert::IsTrue(testCar.currentPath() == expectedPath2);
			Assert::IsTrue(testCar.currentLocation() == 2);

			trafficLightPerceptions = testCar.getTrafficLightPerception();
			Assert::IsTrue(trafficLightPerceptions.first == nullptr);
		}
		
		TEST_METHOD(getPather)
		{
			jw::world::graph_type* testGraph = jw::world::loadWorld(worldRootPath + "/car-unit-test1.json");
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);

			auto testPather = std::make_shared<jw::pathEngine>(testGraphSp);

			jw::car testCar(testPather, nullptr, 1, 2);

			Assert::IsTrue(testCar.pather() == testPather);
		}

		TEST_METHOD(getSetTarget)
		{
			jw::car testCar(nullptr, nullptr, 1, 2, jw::fsm());
			sf::Vector2f targetPosition(5, 3);
			
			auto testFunction = [testCar] { return testCar.targetPosition(); };
			Assert::ExpectException<std::out_of_range>(testFunction);

			testCar.targetPosition(targetPosition);
			Assert::IsTrue(testCar.targetPosition() == targetPosition);
		}

		TEST_METHOD(depth)
		{
			jw::car testCar(nullptr, nullptr, 1, 2);

			Assert::IsTrue(testCar.depth() == 10);
		}

		TEST_METHOD(maxForces)
		{
			jw::car testCar(nullptr, nullptr, 1, 2);

			Assert::IsTrue(testCar.getMaxEngineForce() == 100000.0f);
			Assert::IsTrue(testCar.getMaxBrakeForce() == 200000.0f);
			Assert::IsTrue(testCar.getMass() == 1500.0f);
		}
	
		TEST_METHOD(isAtTarget)
		{
			jw::world::graph_type* testGraph = jw::world::loadWorld(worldRootPath + "/car-unit-test2.json");
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);

			auto testPather = std::make_shared<jw::pathEngine>(testGraphSp);

			jw::car testCar(testPather, nullptr, 1, 2);

			testCar.currentLocation(2);
			testCar.targetPosition(sf::Vector2f(0, 0));

			Assert::IsFalse(testCar.isAtTarget());

			testCar.targetPosition(sf::Vector2f(3, 4));

			Assert::IsTrue(testCar.isAtTarget());
		}

		TEST_METHOD(isAtPosition)
		{
			jw::world::graph_type* testGraph = jw::world::loadWorld(worldRootPath + "/car-unit-test2.json");
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);

			auto testPather = std::make_shared<jw::pathEngine>(testGraphSp);

			jw::car testCar(testPather, nullptr, 1, 2);

			Assert::IsFalse(testCar.isAtPosition(sf::Vector2f(3, 4)));

			testCar.currentLocation(2);

			Assert::IsTrue(testCar.isAtPosition(sf::Vector2f(3, 4)));
		}

		TEST_METHOD(checkEnvironmentLights)
		{
			jw::world::graph_type* testGraph = jw::world::loadWorld(worldRootPath + "/car-unit-test3.json");
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);

			auto testPather = std::make_shared<jw::pathEngine>(testGraphSp);
			std::pair<const sf::Vector2f*, const jw::junctionController::signalState*> trafficLightPerceptions;

			// light ahead
			jw::car testCar(testPather, nullptr, 1, 2, jw::fsm());
			testCar.currentLocation(1);
			testCar.pathTo(2);
			testCar.update(sf::seconds(1));

			trafficLightPerceptions = testCar.getTrafficLightPerception();

			Assert::IsTrue(*trafficLightPerceptions.first == testPather->getRoadEndPosition(testCar.currentLocation(), testCar.currentPath().front()));
			Assert::IsTrue(*trafficLightPerceptions.second == jw::junctionController::signalState::stop);

			// no light ahead
			jw::car testCar2(testPather, nullptr, 1, 2, jw::fsm());
			testCar2.currentLocation(1);
			testCar2.pathTo(3);
			testCar2.update(sf::seconds(1));

			trafficLightPerceptions = testCar2.getTrafficLightPerception();

			Assert::IsTrue(trafficLightPerceptions.first == nullptr);
			Assert::IsTrue(trafficLightPerceptions.second == nullptr);
		}

		TEST_METHOD(checkEnvironmentCarsAhead)
		{
			// TODO how will I do this?
			// is this getting a bit far from a little unit test?
			// are the tests of the collision detection system enough?
		}

		TEST_METHOD(getSituation)
		{
			jw::world::graph_type* testGraph = jw::world::loadWorld(worldRootPath + "/car-unit-test3.json");
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);

			auto testPather = std::make_shared<jw::pathEngine>(testGraphSp);

			jw::car testCar(testPather, nullptr, 1, 2, jw::fsm());

			Assert::IsTrue(testCar.getCurrentSituation() == jw::car::situation::none);

			testCar.currentLocation(1);
			testCar.pathTo(4);
			testCar.targetPosition(testPather->getRoadStartPosition(1, 4));
			testCar.update(sf::milliseconds(10));

			Assert::IsTrue(testCar.getCurrentSituation() == jw::car::situation::inJunction);

			sf::Clock timer;
			while (!testCar.isAtTarget())
			{
				testCar.moveTowardTarget(sf::milliseconds(10));	// update to reach target
				if (timer.getElapsedTime().asSeconds() > 10) Assert::Fail();	// setup failed
			}

			testCar.targetPosition(testPather->getRoadEndPosition(1, 4));
			testCar.update(sf::milliseconds(10));

			Assert::IsTrue(testCar.getCurrentSituation() == jw::car::situation::onRoad);

			timer.restart();
			while (!testCar.isAtTarget())
			{
				testCar.moveTowardTarget(sf::milliseconds(10));	// update to reach target
				if (timer.getElapsedTime().asSeconds() > 10) Assert::Fail();	// setup failed
			}

			testCar.update(sf::milliseconds(10));

			Assert::IsTrue(testCar.getCurrentSituation() == jw::car::situation::atLight);

			testCar.currentLocation(3);

			Assert::IsTrue(testCar.getCurrentSituation() == jw::car::situation::inJunction);
		}

		TEST_METHOD(isTargetClear)
		{
			std::shared_ptr<jw::collisionDetector> testDetector = std::make_shared<jw::collisionDetector>();
			jw::car testCar1(nullptr, testDetector, 1, 2, jw::fsm());
			jw::car testCar2(nullptr, testDetector, 1, 2, jw::fsm());
			testDetector->addCollisionTarget(&testCar1);
			testDetector->addCollisionTarget(&testCar2);

			// no collision
			testCar1.targetPosition(sf::Vector2f(20, 20));
			Assert::IsTrue(testCar1.isTargetClear());

			// collision
			testCar2.setPosition(sf::Vector2f(20, 20));
			Assert::IsFalse(testCar1.isTargetClear());

			// min distance collision
			testCar2.setPosition(sf::Vector2f(27, 20));
			Assert::IsFalse(testCar1.isTargetClear());
		}

		TEST_METHOD(moveTowardTarget)
		{
			Assert::Fail();
		}
	};
}