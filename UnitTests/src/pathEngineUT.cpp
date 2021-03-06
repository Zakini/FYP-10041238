#include "stdafx.h"
#include "CppUnitTest.h"
#include <deque>
#include <SFML/System/Vector2.hpp>
#include "../../Application/src/graph.h"
#include "../../Application/src/world.h"
#include "../../Application/src/location.h"
#include "../../Application/src/pathEngine.h"
#include <memory>
#include "../../Application/src/vectorMaths.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(pathEngine)
	{
	private:
		const string filePathBase = "C:/Users/Josh Wells/Google Drive/Uni/Level 6/Final Year Project/Artefact/data/maps/tests";

	public:
		TEST_METHOD(findPath1)
		{
			// S---T

			jw::pathEngine::graph_type* testGraph = jw::world::loadWorld(filePathBase + "/astar-unit-test1.json");
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);
			jw::pathEngine testPather(testGraphSp);

			std::deque<int> expectedPath;
			expectedPath.emplace_back(2);

			std::deque<int> actualPath = testPather.findPath(1, 2);

			Assert::IsTrue(expectedPath == actualPath);
		}

		TEST_METHOD(findPath2)
		{
			// S-X-T

			jw::pathEngine::graph_type* testGraph = jw::world::loadWorld(filePathBase + "/astar-unit-test2.json");
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);
			jw::pathEngine testPather(testGraphSp);

			std::deque<int> expectedPath;	// Empty path

			std::deque<int> actualPath = testPather.findPath(1, 2);

			Assert::IsTrue(expectedPath == actualPath);
		}

		TEST_METHOD(findPath3)
		{
			// ST

			jw::pathEngine::graph_type* testGraph = jw::world::loadWorld(filePathBase + "/astar-unit-test3.json");
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);
			jw::pathEngine testPather(testGraphSp);

			std::deque<int> expectedPath;	// Empty path

			std::deque<int> actualPath = testPather.findPath(1, 1);

			Assert::IsTrue(expectedPath == actualPath);
		}

		TEST_METHOD(findPath4)
		{
			// S---O---T

			jw::pathEngine::graph_type* testGraph = jw::world::loadWorld(filePathBase + "/astar-unit-test4.json");
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);
			jw::pathEngine testPather(testGraphSp);

			std::deque<int> expectedPath;
			expectedPath.emplace_back(2);
			expectedPath.emplace_back(3);

			std::deque<int> actualPath = testPather.findPath(1, 3);

			Assert::IsTrue(expectedPath == actualPath);
		}

		TEST_METHOD(findPath5)
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

			jw::pathEngine::graph_type* testGraph = jw::world::loadWorld(filePathBase + "/astar-unit-test5.json");
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);
			jw::pathEngine testPather(testGraphSp);

			std::deque<int> expectedPath;
			expectedPath.emplace_back(2);
			expectedPath.emplace_back(5);

			std::deque<int> actualPath = testPather.findPath(1, 5);

			Assert::IsTrue(expectedPath == actualPath);
		}

		TEST_METHOD(findPath6)
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

			jw::pathEngine::graph_type* testGraph = jw::world::loadWorld(filePathBase + "/astar-unit-test6.json");
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);
			jw::pathEngine testPather(testGraphSp);

			std::deque<int> expectedPath;
			expectedPath.emplace_back(2);
			expectedPath.emplace_back(3);	// This could also be 2,-1
			expectedPath.emplace_back(5);
			expectedPath.emplace_back(6);

			std::deque<int> actualPath = testPather.findPath(1, 6);

			Assert::IsTrue(expectedPath == actualPath);
		}

		TEST_METHOD(findPath7)
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

			jw::pathEngine::graph_type* testGraph = jw::world::loadWorld(filePathBase + "/astar-unit-test7.json");
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);
			jw::pathEngine testPather(testGraphSp);

			std::deque<int> expectedPath;	// Empty path

			std::deque<int> actualPath = testPather.findPath(1, 6);

			Assert::IsTrue(expectedPath == actualPath);
		}

		TEST_METHOD(findPath8)
		{
			// S<--T

			jw::pathEngine::graph_type* testGraph = jw::world::loadWorld(filePathBase + "/astar-unit-test8.json");
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);
			jw::pathEngine testPather(testGraphSp);

			std::deque<int> expectedPath;	// Empty path

			std::deque<int> actualPath = testPather.findPath(1, 2);

			Assert::IsTrue(expectedPath == actualPath);
		}

		TEST_METHOD(findPath9)
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

			jw::pathEngine::graph_type* testGraph = jw::world::loadWorld(filePathBase + "/astar-unit-test9.json");
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);
			jw::pathEngine testPather(testGraphSp);

			std::deque<int> expectedPath;
			expectedPath.emplace_back(2);
			expectedPath.emplace_back(4);
			expectedPath.emplace_back(5);
			expectedPath.emplace_back(6);

			std::deque<int> actualPath = testPather.findPath(1, 6);

			Assert::IsTrue(expectedPath == actualPath);
		}

		TEST_METHOD(findPath10)
		{
			// Random generated map

			jw::pathEngine::graph_type* testGraph = jw::world::loadWorld(filePathBase + "/astar-unit-test10.json");
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);
			jw::pathEngine testPather(testGraphSp);

			std::deque<int> expectedPath;
			expectedPath.emplace_back(6);
			expectedPath.emplace_back(1);
			expectedPath.emplace_back(9);

			std::deque<int> actualPath = testPather.findPath(10, 9);

			Assert::IsTrue(expectedPath == actualPath);
		}

		TEST_METHOD(findPath11)
		{
			// no attached graph

			jw::pathEngine testPather(std::shared_ptr<jw::pathEngine::graph_type>(nullptr));
			bool hasThrown = false;

			try { testPather.findPath(1, 2); }	// read access violation, needs /EHa setting to catch
			catch (...) { hasThrown = true; }

			if (!hasThrown) Assert::Fail();
		}

		TEST_METHOD(getLocationPosition)
		{
			jw::pathEngine::graph_type* testGraph = jw::world::loadWorld(filePathBase + "/pather-test1.json");
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);
			jw::pathEngine testPather(testGraphSp);

			Assert::IsTrue(testPather.getLocationPosition(1) == sf::Vector2f(10, -3));
		}
	
		TEST_METHOD(getRoadStartPosition)
		{
			jw::pathEngine::graph_type* testGraph = jw::world::loadWorld(filePathBase + "/pather-test2.json");
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);
			jw::pathEngine testPather(testGraphSp);

			Assert::IsTrue(testPather.getRoadStartPosition(1, 2) == testGraph->edgesAt(1).at(2).startPosition());
			Assert::IsTrue(testPather.getRoadStartPosition(2, 1) == testGraph->edgesAt(2).at(1).startPosition());
		}

		TEST_METHOD(getRoadEndPosition)
		{
			jw::pathEngine::graph_type* testGraph = jw::world::loadWorld(filePathBase + "/pather-test2.json");
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);
			jw::pathEngine testPather(testGraphSp);

			Assert::IsTrue(testPather.getRoadEndPosition(1, 2) == testGraph->edgesAt(1).at(2).endPosition());
			Assert::IsTrue(testPather.getRoadEndPosition(2, 1) == testGraph->edgesAt(2).at(1).endPosition());
		}

		TEST_METHOD(getRoadDirection)
		{
			jw::pathEngine::graph_type* testGraph = jw::world::loadWorld(filePathBase + "/pather-test2.json");
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);
			jw::pathEngine testPather(testGraphSp);
			jw::road& testRoad = testGraph->edgeBetween(1, 2);

			Assert::IsTrue(testPather.getRoadDirection(1, 2) == jw::maths::normalise(testRoad.endPosition() - testRoad.startPosition()));
		}
		
		TEST_METHOD(isLocationTrafficControlled)
		{
			jw::pathEngine::graph_type* testGraph = jw::world::loadWorld(filePathBase + "/pather-test3.json");
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);
			jw::pathEngine testPather(testGraphSp);

			Assert::IsFalse(testPather.isLocationTrafficControlled(1));
			Assert::IsTrue(testPather.isLocationTrafficControlled(2));
		}

		TEST_METHOD(getSignalAtRoadEnd)
		{
			jw::pathEngine::graph_type* testGraph = jw::world::loadWorld(filePathBase + "/pather-test3.json");
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);
			jw::pathEngine testPather(testGraphSp);

			Assert::IsTrue(testPather.getSignalAtRoadEnd(1, 2) == jw::junctionController::signalState::stop);
			Assert::IsTrue(testPather.getSignalAtRoadEnd(2, 1) == jw::junctionController::signalState::go);
		}
	};
}