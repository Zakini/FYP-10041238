#include "stdafx.h"
#include "CppUnitTest.h"
#include <vector>
#include <SFML/System/Vector2.hpp>
#include "../../Application/src/graph.h"
#include "../../Application/src/world.h"
#include "../../Application/src/location.h"
#include "../../Application/src/pathEngine.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(pathEngineTest)
	{
	public:
		TEST_METHOD(findPathTest1)
		{
			jw::world::graph_type testGraph;
			testGraph = jw::world::loadGraph(string("C:/Users/Josh Wells/Google Drive/Uni/Level 6/Final Year Project/Artefact/data/maps/unit-test1.json"));
			
			jw::pathEngine testPather(&testGraph);

			std::vector<sf::Vector2f> expectedPath;
			expectedPath.emplace_back(1, 0);

			// get actual path
			jw::location* start, * target;

			std::vector<sf::Vector2f> actualPath = testPather.findPath(start, target);

			Assert::IsTrue(expectedPath == actualPath);
		}

		TEST_METHOD(findPathTest2)
		{
			// TODO


			Assert::IsTrue(false);
		}

		TEST_METHOD(findPathTest3)
		{
			// TODO


			Assert::IsTrue(false);
		}

		TEST_METHOD(findPathTest4)
		{
			// TODO


			Assert::IsTrue(false);
		}

		TEST_METHOD(findPathTest5)
		{
			// TODO


			Assert::IsTrue(false);
		}

		TEST_METHOD(findPathTest6)
		{
			// TODO


			Assert::IsTrue(false);
		}

		TEST_METHOD(findPathTest7)
		{
			// TODO


			Assert::IsTrue(false);
		}

		TEST_METHOD(findPathTest8)
		{
			// TODO


			Assert::IsTrue(false);
		}

		TEST_METHOD(findPathTest9)
		{
			// TODO


			Assert::IsTrue(false);
		}

		TEST_METHOD(findPathTest10)
		{
			// TODO


			Assert::IsTrue(false);
		}

		TEST_METHOD(findPathTest11)
		{
			// TODO


			Assert::IsTrue(false);
		}

		TEST_METHOD(findPathTest12)
		{
			// TODO


			Assert::IsTrue(false);
		}
	};
}