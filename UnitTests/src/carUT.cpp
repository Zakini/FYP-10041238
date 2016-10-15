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
	public:
		TEST_METHOD(loadFromString)	// Tests homeLocation and workLocation methods too
		{
			std::string carJsonFilePath = "C:/Users/Josh Wells/Google Drive/Uni/Level 6/Final Year Project/Artefact/data/entities/load-cars-unit-test1.json";
			std::string worldJsonFilePath = "C:/Users/Josh Wells/Google Drive/Uni/Level 6/Final Year Project/Artefact/data/maps/car-unit-test1.json";
			jw::world::graph_type* testGraph = jw::world::loadWorld(worldJsonFilePath);
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);
			auto testPather = std::make_shared<jw::pathEngine>(testGraphSp);
			auto testDetector = std::make_shared<jw::collisionDetector>();
			std::vector<jw::car*> testCars = jw::car::loadCars(carJsonFilePath, testPather, testDetector);

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
			std::ifstream testFile("C:/Users/Josh Wells/Google Drive/Uni/Level 6/Final Year Project/Artefact/data/entities/load-cars-unit-test1.json");
			std::string worldJsonFilePath = "C:/Users/Josh Wells/Google Drive/Uni/Level 6/Final Year Project/Artefact/data/maps/car-unit-test1.json";
			jw::world::graph_type* testGraph = jw::world::loadWorld(worldJsonFilePath);
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

		TEST_METHOD(getPosition)
		{
			string worldJsonFilePath = "C:/Users/Josh Wells/Google Drive/Uni/Level 6/Final Year Project/Artefact/data/maps/car-unit-test2.json";
			jw::world::graph_type* testGraph = jw::world::loadWorld(worldJsonFilePath);
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);

			auto testPather = std::make_shared<jw::pathEngine>(testGraphSp);

			jw::car testCar(testPather, nullptr, 1, 2);

			testCar.currentLocation(2);

			Assert::IsTrue(testCar.getPosition() == sf::Vector2f(3, 4));
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

			testCar.update(updateLength);

			Assert::IsTrue(testCar.getVelocity() == expectedVelocity);
		}

		TEST_METHOD(getHeading)
		{
			jw::car testCar(nullptr, nullptr, 1, 2, jw::fsm());
			sf::Vector2f targetPosition(10000, 0);
			testCar.targetPosition(targetPosition);

			testCar.update(sf::seconds(1));

			Assert::IsTrue(testCar.getHeading() == sf::Vector2f(1, 0));

			//float maxDeceleration = testCar.getMaxBrakeForce() / testCar.getMass();
			//float currentSpeed = jw::maths::length(testCar.getVelocity());
			//float closestStopDistance = (-(currentSpeed * currentSpeed)) / (2 * -maxDeceleration) * 2.0f;	// +100% buffer
			//sf::Vector2f closestStopPosition = testCar.getPosition() + testCar.getHeading() * closestStopDistance;

			testCar.targetPosition(testCar.getPosition());

			//sf::Time minTimeToFullStop = sf::seconds((-currentSpeed) / (-(maxDeceleration * 2.0f)));	// +100% buffer
			//sf::Time step = sf::milliseconds(10);

			for (int i = 0; i < 1000; i++)
			{
				testCar.update(sf::milliseconds(10));
				if (jw::maths::length(testCar.getVelocity()) == 0) break;
			}

			Assert::IsTrue(jw::maths::length(testCar.getVelocity()) == 0);	// setup has failed, not getHeading
			Assert::IsTrue(testCar.getHeading() == sf::Vector2f(1, 0));
		}

		TEST_METHOD(getSetCurrentLocationId)
		{
			string worldJsonFilePath = "C:/Users/Josh Wells/Google Drive/Uni/Level 6/Final Year Project/Artefact/data/maps/car-unit-test2.json";
			jw::world::graph_type* testGraph = jw::world::loadWorld(worldJsonFilePath);
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);

			auto testPather = std::make_shared<jw::pathEngine>(testGraphSp);

			jw::car testCar(testPather, nullptr, 1, 2);

			testCar.currentLocation(2);

			Assert::IsTrue(testCar.currentLocation() == 2);
		}
	
		TEST_METHOD(pathToUpdateAndGetPath)
		{
			string worldJsonFilePath = "C:/Users/Josh Wells/Google Drive/Uni/Level 6/Final Year Project/Artefact/data/maps/car-unit-test2.json";
			jw::world::graph_type* testGraph = jw::world::loadWorld(worldJsonFilePath);
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);

			auto testPather = std::make_shared<jw::pathEngine>(testGraphSp);

			jw::car testCar(testPather, nullptr, 1, 2);

			testCar.currentLocation(1);
			testCar.pathTo(2);

			deque<int> expectedPath1, expectedPath2;
			expectedPath1.push_back(2);	// maybe use more complicated map for a longer route?

			Assert::IsTrue(testCar.currentPath() == expectedPath1);
			testCar.popStepFromPath();
			Assert::IsTrue(testCar.currentPath() == expectedPath2);
			Assert::IsTrue(testCar.currentLocation() == 2);
		}
		
		TEST_METHOD(getPather)
		{
			string worldJsonFilePath = "C:/Users/Josh Wells/Google Drive/Uni/Level 6/Final Year Project/Artefact/data/maps/car-unit-test1.json";
			jw::world::graph_type* testGraph = jw::world::loadWorld(worldJsonFilePath);
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
			string worldJsonFilePath = "C:/Users/Josh Wells/Google Drive/Uni/Level 6/Final Year Project/Artefact/data/maps/car-unit-test2.json";
			jw::world::graph_type* testGraph = jw::world::loadWorld(worldJsonFilePath);
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
			string worldJsonFilePath = "C:/Users/Josh Wells/Google Drive/Uni/Level 6/Final Year Project/Artefact/data/maps/car-unit-test2.json";
			jw::world::graph_type* testGraph = jw::world::loadWorld(worldJsonFilePath);
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);

			auto testPather = std::make_shared<jw::pathEngine>(testGraphSp);

			jw::car testCar(testPather, nullptr, 1, 2);

			Assert::IsFalse(testCar.isAtPosition(sf::Vector2f(3, 4)));

			testCar.currentLocation(2);

			Assert::IsTrue(testCar.isAtPosition(sf::Vector2f(3, 4)));
		}

		TEST_METHOD(checkEnvironmentLights)
		{
			// traffic lights
			string worldJsonFilePath = "C:/Users/Josh Wells/Google Drive/Uni/Level 6/Final Year Project/Artefact/data/maps/car-unit-test3.json";
			jw::world::graph_type* testGraph = jw::world::loadWorld(worldJsonFilePath);
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);

			auto testPather = std::make_shared<jw::pathEngine>(testGraphSp);
			std::pair<const sf::Vector2f*, const jw::junctionController::signalState*> trafficLightPerceptions;

			// light ahead, not moving towards it
			jw::car testCar1(testPather, nullptr, 1, 2, jw::fsm());
			testCar1.currentLocation(1);
			testCar1.pathTo(2);
			testCar1.update(sf::seconds(1));

			trafficLightPerceptions = testCar1.getTrafficLightPerception();

			Assert::IsTrue(*trafficLightPerceptions.first == testPather->getRoadEndPosition(1, 4));
			Assert::IsTrue(trafficLightPerceptions.second == nullptr);

			// light ahead, moving towards it
			jw::car testCar2(testPather, nullptr, 1, 2, jw::fsm());
			testCar2.currentLocation(1);
			testCar2.pathTo(2);
			testCar2.targetPosition(testPather->getRoadEndPosition(1, 4));
			testCar2.update(sf::seconds(1));

			trafficLightPerceptions = testCar2.getTrafficLightPerception();

			Assert::IsTrue(*trafficLightPerceptions.first == testPather->getRoadEndPosition(testCar2.currentLocation(), testCar2.currentPath().front()));
			Assert::IsTrue(*trafficLightPerceptions.second == jw::junctionController::signalState::stop);

			// no light ahead, not moving towards it
			jw::car testCar3(testPather, nullptr, 1, 2, jw::fsm());
			testCar3.currentLocation(1);
			testCar3.pathTo(3);
			testCar3.update(sf::seconds(1));

			trafficLightPerceptions = testCar3.getTrafficLightPerception();

			Assert::IsTrue(trafficLightPerceptions.first == nullptr);
			Assert::IsTrue(trafficLightPerceptions.second == nullptr);

			// no light ahead, moving towards it
			jw::car testCar4(testPather, nullptr, 1, 2, jw::fsm());
			testCar4.currentLocation(1);
			testCar4.pathTo(3);
			testCar4.targetPosition(testPather->getRoadEndPosition(testCar2.currentLocation(), testCar2.currentPath().front()));
			testCar4.update(sf::seconds(1));

			trafficLightPerceptions = testCar4.getTrafficLightPerception();

			Assert::IsTrue(trafficLightPerceptions.first == nullptr);
			Assert::IsTrue(trafficLightPerceptions.second == nullptr);
		}

		TEST_METHOD(checkEnvironmentCarsAhead)
		{
			// TODO how will I do this?
			// is this getting a bit far from a little unit test?
			// are the tests of the collision detection system enough?
		}
	};
}