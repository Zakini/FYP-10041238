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
			jw::pathEngine testPather(testGraphSp);
			std::vector<jw::car*> testCars = jw::car::loadCars(carJsonFilePath, &testPather);

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
			jw::pathEngine testPather(testGraphSp);
			nlohmann::json testJson;

			testFile >> testJson;

			std::vector<jw::car*> testCars = jw::car::loadCars(testJson, &testPather);

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
			jw::car testCar(nullptr, 1, 2);	// nullptr is invalid but works for this test

			Assert::IsTrue(testCar.homeLocation() == 1);
		}

		TEST_METHOD(getWorkLocationId)
		{
			jw::car testCar(nullptr, 1, 2);	// nullptr is invalid but works for this test

			Assert::IsTrue(testCar.workLocation() == 2);
		}

		TEST_METHOD(getPosition)
		{
			string worldJsonFilePath = "C:/Users/Josh Wells/Google Drive/Uni/Level 6/Final Year Project/Artefact/data/maps/car-unit-test2.json";
			jw::world::graph_type* testGraph = jw::world::loadWorld(worldJsonFilePath);
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);

			jw::pathEngine testPather(testGraphSp);

			jw::car testCar(&testPather, 1, 2);

			testCar.currentLocation(2);

			Assert::IsTrue(testCar.position() == sf::Vector2f(3, 4));
		}

		TEST_METHOD(getVelocity)
		{
			jw::car testCar(nullptr, 1, 2, jw::fsm());
			sf::Vector2f targetPosition(10000, 0);
			testCar.targetPosition(targetPosition);

			// TODO get rid of magic numbers, use getters
			sf::Time updateLength = sf::milliseconds(10);
			sf::Vector2f expectedDirection = jw::maths::normalise(targetPosition);
			sf::Vector2f expectedForce = 100000.0f * expectedDirection;	// no friction since car is not moving yet
			sf::Vector2f expectedAcceleration = expectedForce / 1.0f;
			sf::Vector2f expectedVelocity = expectedAcceleration * updateLength.asSeconds();

			testCar.update(updateLength);

			Assert::IsTrue(testCar.velocity() == expectedVelocity);
		}

		TEST_METHOD(getSetCurrentLocationId)
		{
			string worldJsonFilePath = "C:/Users/Josh Wells/Google Drive/Uni/Level 6/Final Year Project/Artefact/data/maps/car-unit-test2.json";
			jw::world::graph_type* testGraph = jw::world::loadWorld(worldJsonFilePath);
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);

			jw::pathEngine testPather(testGraphSp);

			jw::car testCar(&testPather, 1, 2);

			testCar.currentLocation(2);

			Assert::IsTrue(testCar.currentLocation() == 2);
		}
	
		TEST_METHOD(pathToUpdateAndGetPath)
		{
			string worldJsonFilePath = "C:/Users/Josh Wells/Google Drive/Uni/Level 6/Final Year Project/Artefact/data/maps/car-unit-test2.json";
			jw::world::graph_type* testGraph = jw::world::loadWorld(worldJsonFilePath);
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);

			jw::pathEngine testPather(testGraphSp);

			jw::car testCar(&testPather, 1, 2);

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

			jw::pathEngine testPather(testGraphSp);

			jw::car testCar(&testPather, 1, 2);

			Assert::IsTrue(testCar.pather() == &testPather);
		}

		TEST_METHOD(getSetTarget)
		{
			jw::car testCar(nullptr, 1, 2, jw::fsm());
			sf::Vector2f targetPosition(5, 3);
			
			auto testFunction = [testCar] { return testCar.targetPosition(); };
			Assert::ExpectException<std::out_of_range>(testFunction);

			testCar.targetPosition(targetPosition);
			Assert::IsTrue(testCar.targetPosition() == targetPosition);
		}

		TEST_METHOD(depth)
		{
			jw::car testCar(nullptr, 1, 2);

			Assert::IsTrue(testCar.depth() == 10);
		}
	};
}