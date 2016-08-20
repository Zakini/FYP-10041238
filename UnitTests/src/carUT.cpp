#include "stdafx.h"
#include "CppUnitTest.h"

#include "../../Application/src/car.h"
#include <vector>
#include <string>
#include "../../Application/src/world.h"
#include "../../Application/src/pathEngine.h"
#include <fstream>
#include "../../Application/lib/json-master/src/json.hpp"

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
			jw::world::graph_type testGraph = jw::world::loadWorld(worldJsonFilePath);
			jw::pathEngine testPather(&testGraph);
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
			jw::world::graph_type testGraph = jw::world::loadWorld(worldJsonFilePath);
			jw::pathEngine testPather(&testGraph);
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
			// TODO
		}

		TEST_METHOD(getWorkLocationId)
		{
			// TODO
		}
	};
}