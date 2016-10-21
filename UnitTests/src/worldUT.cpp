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

			// node 1
			Assert::IsTrue(testGraph->nodeAt(1).position() == sf::Vector2f(0, 1));
			Assert::IsFalse(testGraph->nodeAt(1).isControlledJunction());
			// node 1 edges
			Assert::IsTrue(testGraph->edgeBetween(1, 5).cost() == 1);

			// node 2
			Assert::IsTrue(testGraph->nodeAt(2).position() == sf::Vector2f(3, 1));
			Assert::IsFalse(testGraph->nodeAt(2).isControlledJunction());
			// node 2 edges
			Assert::IsTrue(testGraph->edgeBetween(2, 5).cost() == 2);
			
			// node 3
			Assert::IsTrue(testGraph->nodeAt(3).position() == sf::Vector2f(1, -2));
			Assert::IsFalse(testGraph->nodeAt(3).isControlledJunction());
			// node 3 edges
			Assert::IsTrue(testGraph->edgeBetween(3, 5).cost() == 3);

			// node 4
			Assert::IsTrue(testGraph->nodeAt(4).position() == sf::Vector2f(1, 5));
			Assert::IsFalse(testGraph->nodeAt(4).isControlledJunction());
			// node 4 edges
			Assert::IsTrue(testGraph->edgeBetween(4, 5).cost() == 4);

			// node 5
			Assert::IsTrue(testGraph->nodeAt(5).position() == sf::Vector2f(1, 1));
			Assert::IsTrue(testGraph->nodeAt(5).isControlledJunction());
			// node 5 edges
			Assert::IsTrue(testGraph->edgeBetween(5, 1).cost() == 1);
			Assert::IsTrue(testGraph->edgeBetween(5, 2).cost() == 2);
			Assert::IsTrue(testGraph->edgeBetween(5, 3).cost() == 3);
			Assert::IsTrue(testGraph->edgeBetween(5, 4).cost() == 4);
		}

		TEST_METHOD(loadGraphJson)
		{
			std::ifstream testFile(rootPath + "/load-unit-test1.json");
			nlohmann::json testJson;

			testFile >> testJson;

			jw::world::graph_type* testGraph = jw::world::loadWorld(testJson);

			// node 1
			Assert::IsTrue(testGraph->nodeAt(1).position() == sf::Vector2f(0, 1));
			Assert::IsFalse(testGraph->nodeAt(1).isControlledJunction());
			// node 1 edges
			Assert::IsTrue(testGraph->edgeBetween(1, 5).cost() == 1);

			// node 2
			Assert::IsTrue(testGraph->nodeAt(2).position() == sf::Vector2f(3, 1));
			Assert::IsFalse(testGraph->nodeAt(2).isControlledJunction());
			// node 2 edges
			Assert::IsTrue(testGraph->edgeBetween(2, 5).cost() == 2);

			// node 3
			Assert::IsTrue(testGraph->nodeAt(3).position() == sf::Vector2f(1, -2));
			Assert::IsFalse(testGraph->nodeAt(3).isControlledJunction());
			// node 3 edges
			Assert::IsTrue(testGraph->edgeBetween(3, 5).cost() == 3);

			// node 4
			Assert::IsTrue(testGraph->nodeAt(4).position() == sf::Vector2f(1, 5));
			Assert::IsFalse(testGraph->nodeAt(4).isControlledJunction());
			// node 4 edges
			Assert::IsTrue(testGraph->edgeBetween(4, 5).cost() == 4);

			// node 5
			Assert::IsTrue(testGraph->nodeAt(5).position() == sf::Vector2f(1, 1));
			Assert::IsTrue(testGraph->nodeAt(5).isControlledJunction());
			// node 5 edges
			Assert::IsTrue(testGraph->edgeBetween(5, 1).cost() == 1);
			Assert::IsTrue(testGraph->edgeBetween(5, 2).cost() == 2);
			Assert::IsTrue(testGraph->edgeBetween(5, 3).cost() == 3);
			Assert::IsTrue(testGraph->edgeBetween(5, 4).cost() == 4);
		}
	};
}