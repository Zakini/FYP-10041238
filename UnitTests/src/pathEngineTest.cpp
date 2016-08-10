#include "stdafx.h"
#include "CppUnitTest.h"
#include <deque>
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
	private:
		const string filePathBase = "C:/Users/Josh Wells/Google Drive/Uni/Level 6/Final Year Project/Artefact/data/maps/";

	public:
		TEST_METHOD(findPathTest1)
		{
			// S---T

			jw::world::graph_type testGraph;
			testGraph = jw::world::loadWorld(filePathBase + "astar-unit-test1.json");
			
			jw::pathEngine testPather(&testGraph);

			std::deque<int> expectedPath;
			expectedPath.emplace_back(2);

			std::deque<int> actualPath = testPather.findPath(1, 2);

			Assert::IsTrue(expectedPath == actualPath);
		}

		TEST_METHOD(findPathTest2)
		{
			// S-X-T

			jw::world::graph_type testGraph;
			testGraph = jw::world::loadWorld(filePathBase + "astar-unit-test2.json");

			jw::pathEngine testPather(&testGraph);

			std::deque<int> expectedPath;	// Empty path

			std::deque<int> actualPath = testPather.findPath(1, 2);

			Assert::IsTrue(expectedPath == actualPath);
		}

		TEST_METHOD(findPathTest3)
		{
			// ST

			jw::world::graph_type testGraph;
			testGraph = jw::world::loadWorld(filePathBase + "astar-unit-test3.json");

			jw::pathEngine testPather(&testGraph);

			std::deque<int> expectedPath;	// Empty path

			std::deque<int> actualPath = testPather.findPath(1, 1);

			Assert::IsTrue(expectedPath == actualPath);
		}

		TEST_METHOD(findPathTest4)
		{
			// S---O---T

			jw::world::graph_type testGraph;
			testGraph = jw::world::loadWorld(filePathBase + "astar-unit-test4.json");

			jw::pathEngine testPather(&testGraph);

			std::deque<int> expectedPath;
			expectedPath.emplace_back(2);
			expectedPath.emplace_back(3);

			std::deque<int> actualPath = testPather.findPath(1, 3);

			Assert::IsTrue(expectedPath == actualPath);
		}

		TEST_METHOD(findPathTest5)
		{
			//     O
			//    / \
			//   /   \
			//  /     \
			// S---O---T
			//  \     /
			//   \   /
			//    \ /
			//     O

			jw::world::graph_type testGraph;
			testGraph = jw::world::loadWorld(filePathBase + "astar-unit-test5.json");

			jw::pathEngine testPather(&testGraph);

			std::deque<int> expectedPath;
			expectedPath.emplace_back(2);
			expectedPath.emplace_back(5);

			std::deque<int> actualPath = testPather.findPath(1, 5);

			Assert::IsTrue(expectedPath == actualPath);
		}

		TEST_METHOD(findPathTest6)
		{
			//         O
			//        / \
			//       /   \
			//      /     \
			// S---O       O-------T
			//      \     /
			//       \   /
			//        \ /
			//         O

			jw::world::graph_type testGraph;
			testGraph = jw::world::loadWorld(filePathBase + "astar-unit-test6.json");

			jw::pathEngine testPather(&testGraph);

			std::deque<int> expectedPath;
			expectedPath.emplace_back(2);
			expectedPath.emplace_back(3);	// This could also be 2,-1
			expectedPath.emplace_back(5);
			expectedPath.emplace_back(6);

			std::deque<int> actualPath = testPather.findPath(1, 6);

			Assert::IsTrue(expectedPath == actualPath);
		}

		TEST_METHOD(findPathTest7)
		{
			//         O
			//        / \
			//       /   \
			//      /     \
			// S---O       O---X---T
			//      \     /
			//       \   /
			//        \ /
			//         O

			jw::world::graph_type testGraph;
			testGraph = jw::world::loadWorld(filePathBase + "astar-unit-test7.json");

			jw::pathEngine testPather(&testGraph);

			std::deque<int> expectedPath;	// Empty path

			std::deque<int> actualPath = testPather.findPath(1, 6);

			Assert::IsTrue(expectedPath == actualPath);
		}

		TEST_METHOD(findPathTest8)
		{
			// S<--T

			jw::world::graph_type testGraph;
			testGraph = jw::world::loadWorld(filePathBase + "astar-unit-test8.json");

			jw::pathEngine testPather(&testGraph);

			std::deque<int> expectedPath;	// Empty path

			std::deque<int> actualPath = testPather.findPath(1, 2);

			Assert::IsTrue(expectedPath == actualPath);
		}

		TEST_METHOD(findPathTest9)
		{
			//             O
			//           /   ^
			//         /       \
			//       V           \
			// S---O               O-------T
			//      \             ^
			//       \           /
			//        \         /
			//         \       /
			//          \     /
			//           \   /
			//            V /
			//             O

			jw::world::graph_type testGraph;
			testGraph = jw::world::loadWorld(filePathBase + "astar-unit-test9.json");

			jw::pathEngine testPather(&testGraph);

			std::deque<int> expectedPath;
			expectedPath.emplace_back(2);
			expectedPath.emplace_back(4);
			expectedPath.emplace_back(5);
			expectedPath.emplace_back(6);

			std::deque<int> actualPath = testPather.findPath(1, 6);

			Assert::IsTrue(expectedPath == actualPath);
		}

		TEST_METHOD(findPathTest10)
		{
			// Random generated map

			jw::world::graph_type testGraph;
			testGraph = jw::world::loadWorld(filePathBase + "astar-unit-test10.json");

			jw::pathEngine testPather(&testGraph);

			std::deque<int> expectedPath;
			expectedPath.emplace_back(6);
			expectedPath.emplace_back(1);
			expectedPath.emplace_back(9);

			std::deque<int> actualPath = testPather.findPath(10, 9);

			Assert::IsTrue(expectedPath == actualPath);
		}
	};
}