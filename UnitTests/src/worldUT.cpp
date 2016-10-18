#include "stdafx.h"
#include "CppUnitTest.h"

#include "../../Application/src/world.h"
#include <SFML/System/Vector2.hpp>
#include <string>
#include <fstream>
#include "../../Application/lib/json-master/src/json.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(world)
	{
	private:
		const string rootPath = "C:/Users/Josh Wells/Google Drive/Uni/Level 6/Final Year Project/Artefact/data/maps/tests";

	public:
		TEST_METHOD(loadGraphString)
		{
			jw::world::graph_type* testGraph = jw::world::loadWorld(rootPath + "/load-unit-test1.json");

			// Assert node 1 is at 0,0
			Assert::IsTrue(testGraph->nodeAt(1).position() == sf::Vector2f(0, 0));
			// Assert node 1 has 1 edge to 2
			Assert::IsTrue(testGraph->edgesAt(1).at(2).cost() == 1);

			// Assert node 2 is at 1,0
			Assert::IsTrue(testGraph->nodeAt(2).position() == sf::Vector2f(1, 0));
			// Assert node 2 has 1 edge to 1
			Assert::IsTrue(testGraph->edgesAt(2).at(1).cost() == 1);

			Assert::Fail();	// TODO test too simple
		}

		TEST_METHOD(loadGraphJson)
		{
			std::ifstream testFile(rootPath + "/load-unit-test1.json");
			nlohmann::json testJson;

			testFile >> testJson;

			jw::world::graph_type* testGraph = jw::world::loadWorld(testJson);

			// Assert node 1 is at 0,0
			Assert::IsTrue(testGraph->nodeAt(1).position() == sf::Vector2f(0, 0));
			// Assert node 1 has 1 edge to 2
			Assert::IsTrue(testGraph->edgesAt(1).at(2).cost() == 1);

			// Assert node 2 is at 1,0
			Assert::IsTrue(testGraph->nodeAt(2).position() == sf::Vector2f(1, 0));
			// Assert node 2 has 1 edge to 1
			Assert::IsTrue(testGraph->edgesAt(2).at(1).cost() == 1);

			Assert::Fail();	// TODO test too simple
		}
	};
}