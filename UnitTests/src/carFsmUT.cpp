#include "stdafx.h"
#include "CppUnitTest.h"
#include "../../Application/src/car.h"
#include "../../Application/src/pathEngine.h"
#include "../../Application/src/carFsm.h"
#include <SFML/System/Time.hpp>
#include "../../Application/src/world.h"
#include <deque>
#include <SFML/System/Clock.hpp>
#include "../../Application/src/finiteStateMachine.h"
#include <stdexcept>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using std::out_of_range;

namespace UnitTests
{
	TEST_CLASS(carFsm)
	{
	public:
		TEST_METHOD(moveToHome)
		{
			string worldJsonFilePath = "C:/Users/Josh Wells/Google Drive/Uni/Level 6/Final Year Project/Artefact/data/maps/carfsm-unit-test1.json";
			jw::world::graph_type* testGraph = jw::world::loadWorld(worldJsonFilePath);
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);

			jw::pathEngine pather(testGraphSp);

			jw::car testCar(&pather, 1, 2, jw::fsm());
			jw::carFsm::moveToHome testState(testCar);

			testState.update(sf::seconds(0));	// move should be instantaneous

			Assert::IsTrue(testCar.position() == pather.getLocationPosition(1));
			Assert::IsTrue(testCar.currentLocation() == 1);
		}

		TEST_METHOD(pathToHome)
		{
			string worldJsonFilePath = "C:/Users/Josh Wells/Google Drive/Uni/Level 6/Final Year Project/Artefact/data/maps/carfsm-unit-test1.json";
			jw::world::graph_type* testGraph = jw::world::loadWorld(worldJsonFilePath);
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);

			jw::pathEngine pather(testGraphSp);

			jw::car testCar(&pather, 1, 2, jw::fsm());
			jw::carFsm::pathToHome testState(testCar);

			testCar.currentLocation(2);			// move to non-home location
			testState.update(sf::seconds(0));	// path generation is not dependant on world time

			std::deque<int> expectedPath;
			expectedPath.push_back(1);

			Assert::IsTrue(testCar.currentPath() == expectedPath);
		}

		TEST_METHOD(pathToWork)
		{
			string worldJsonFilePath = "C:/Users/Josh Wells/Google Drive/Uni/Level 6/Final Year Project/Artefact/data/maps/carfsm-unit-test1.json";
			jw::world::graph_type* testGraph = jw::world::loadWorld(worldJsonFilePath);
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);

			jw::pathEngine pather(testGraphSp);

			jw::car testCar(&pather, 1, 2, jw::fsm());
			jw::carFsm::pathToWork testState(testCar);

			testCar.currentLocation(1);			// move to non-work location
			testState.update(sf::seconds(0));	// path generation is not dependant on world time

			std::deque<int> expectedPath;
			expectedPath.push_back(2);

			Assert::IsTrue(testCar.currentPath() == expectedPath);
		}
		
		TEST_METHOD(targetRoadStart)
		{
			string worldJsonFilePath = "C:/Users/Josh Wells/Google Drive/Uni/Level 6/Final Year Project/Artefact/data/maps/carfsm-unit-test2.json";
			jw::world::graph_type* testGraph = jw::world::loadWorld(worldJsonFilePath);
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);

			jw::pathEngine pather(testGraphSp);

			jw::car testCar(&pather, 1, 2, jw::fsm());
			testCar.targetPosition(pather.getRoadEndPosition(1, 2));
			jw::carFsm::targetRoadStart testState(testCar);

			testCar.currentLocation(1);	// move to non-work location
			testCar.pathTo(2);			// generate path to work

			testState.update(sf::seconds(0));	// not dependent on world time

			Assert::IsTrue(testCar.targetPosition() == pather.getRoadStartPosition(1, 2));
		}

		TEST_METHOD(targetRoadEnd)
		{
			string worldJsonFilePath = "C:/Users/Josh Wells/Google Drive/Uni/Level 6/Final Year Project/Artefact/data/maps/carfsm-unit-test2.json";
			jw::world::graph_type* testGraph = jw::world::loadWorld(worldJsonFilePath);
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);

			jw::pathEngine pather(testGraphSp);

			jw::car testCar(&pather, 1, 2, jw::fsm());
			testCar.targetPosition(pather.getRoadEndPosition(1, 2));
			jw::carFsm::targetRoadEnd testState(testCar);

			testCar.currentLocation(1);	// move to non-work location
			testCar.pathTo(2);			// generate path to work

			testState.update(sf::seconds(0));	// not dependent on world time

			Assert::IsTrue(testCar.targetPosition() == pather.getRoadEndPosition(1, 2));
		}

		TEST_METHOD(updatePath)
		{
			string worldJsonFilePath = "C:/Users/Josh Wells/Google Drive/Uni/Level 6/Final Year Project/Artefact/data/maps/carfsm-unit-test2.json";
			jw::world::graph_type* testGraph = jw::world::loadWorld(worldJsonFilePath);
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);

			jw::pathEngine pather(testGraphSp);

			jw::car testCar(&pather, 1, 2, jw::fsm());
			testCar.currentLocation(1);
			testCar.pathTo(2);
			testCar.targetPosition(pather.getRoadEndPosition(1, 2));

			deque<int> initialPath = testCar.currentPath();
			jw::carFsm::atTarget setupTransition(testCar);
			jw::carFsm::updatePath testState(testCar);
			testState.update(sf::seconds(0));

			Assert::IsTrue(initialPath == testCar.currentPath());	// we haven't moved. path should be the same

			sf::Clock timer;	// start

			while (!setupTransition.changeState())
			{
				// repeated update until we arrive at the location
				testCar.update(sf::milliseconds(10));	// ~60fps

				if (timer.getElapsedTime().asSeconds() >= 10)	// 10 sec time limit
				{
					Assert::Fail();
				}
			}

			testState.update(sf::seconds(0));
			initialPath.pop_front();
			Assert::IsTrue(initialPath == testCar.currentPath());
		}

		TEST_METHOD(arrived)
		{
			string worldJsonFilePath = "C:/Users/Josh Wells/Google Drive/Uni/Level 6/Final Year Project/Artefact/data/maps/carfsm-unit-test2.json";
			jw::world::graph_type* testGraph = jw::world::loadWorld(worldJsonFilePath);
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);

			jw::pathEngine pather(testGraphSp);

			jw::car testCar(&pather, 1, 2, jw::fsm());
			testCar.currentLocation(1);
			testCar.targetPosition(pather.getRoadEndPosition(1, 2));
			jw::carFsm::arrived testState(testCar);

			sf::Clock timer;	// start

			while (!testState.changeState())
			{
				// repeated update until we arrive at the location
				testCar.update(sf::milliseconds(10));	// ~60fps

				if (timer.getElapsedTime().asSeconds() >= 10)	// 10 sec time limit
				{
					Assert::Fail();
				}
			}

			// pass
		}

		TEST_METHOD(atTarget)
		{
			string worldJsonFilePath = "C:/Users/Josh Wells/Google Drive/Uni/Level 6/Final Year Project/Artefact/data/maps/carfsm-unit-test2.json";
			jw::world::graph_type* testGraph = jw::world::loadWorld(worldJsonFilePath);
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);

			jw::pathEngine pather(testGraphSp);

			jw::car testCar(&pather, 1, 2, jw::fsm());
			testCar.targetPosition(pather.getRoadEndPosition(1, 2));
			testCar.currentLocation(1);
			jw::carFsm::atTarget testState(testCar);

			sf::Clock timer;	// start

			while (!testState.changeState())
			{
				// repeated update until we arrive at the location
				testCar.update(sf::milliseconds(10));	// ~60fps

				if (timer.getElapsedTime().asSeconds() >= 10)	// 10 sec time limit
				{
					Assert::Fail();
				}
			}

			// pass
		}

		TEST_METHOD(generate)
		{
			jw::car testCar(nullptr, 1, 2);
			jw::fsm testFsm = jw::carFsm::generate(testCar);
			jw::fsm::state_type testState;
			jw::fsm::transition_type testTransition;

			// ###############
			// ### state 1 ###
			// ###############

			// check exists
			try { testState = testFsm.fsmState(1); }
			catch (...)	{ Assert::Fail(); }

			// and is a nullState
			if (dynamic_cast<jw::nullState*>(testState) == nullptr) Assert::Fail();

			// and it has a transition to state 2, priority 1
			try { testTransition = testFsm.fsmTransition(1, 2, 1); }
			catch (...) { Assert::Fail(); }

			// and it is a nullTransition
			if (dynamic_cast<jw::nullTransition*>(testTransition) == nullptr) Assert::Fail();

			// ###############
			// ### state 2 ###
			// ###############

			// check exists
			try { testState = testFsm.fsmState(2); }
			catch (...) { Assert::Fail(); }

			// and is a moveToHome
			if (dynamic_cast<jw::carFsm::moveToHome*>(testState) == nullptr) Assert::Fail();

			// and it has a transition to state 3, priority 1
			try { testTransition = testFsm.fsmTransition(2, 3, 1); }
			catch (...) { Assert::Fail(); }

			// and it is a nullTransition
			if (dynamic_cast<jw::nullTransition*>(testTransition) == nullptr) Assert::Fail();

			// ###############
			// ### state 3 ###
			// ###############

			// check exists
			try { testState = testFsm.fsmState(3); }
			catch (...) { Assert::Fail(); }

			// and is a pathToWork
			if (dynamic_cast<jw::carFsm::pathToWork*>(testState) == nullptr) Assert::Fail();

			// and it has a transition to state 4, priority 1
			try { testTransition = testFsm.fsmTransition(3, 4, 1); }
			catch (...) { Assert::Fail(); }

			// and it is a nullTransition
			if (dynamic_cast<jw::nullTransition*>(testTransition) == nullptr) Assert::Fail();

			// ###############
			// ### state 4 ###
			// ###############

			// check exists
			try { testState = testFsm.fsmState(4); }
			catch (...) { Assert::Fail(); }

			// and is a targetRoadStart
			if (dynamic_cast<jw::carFsm::targetRoadStart*>(testState) == nullptr) Assert::Fail();

			// and it has a transition to state 5, priority 1
			try { testTransition = testFsm.fsmTransition(4, 5, 1); }
			catch (...) { Assert::Fail(); }

			// and it is an atTarget
			if (dynamic_cast<jw::carFsm::atTarget*>(testTransition) == nullptr) Assert::Fail();

			// ###############
			// ### state 5 ###
			// ###############

			// check exists
			try { testState = testFsm.fsmState(5); }
			catch (...) { Assert::Fail(); }

			// and is a targetRoadEnd
			if (dynamic_cast<jw::carFsm::targetRoadEnd*>(testState) == nullptr) Assert::Fail();

			// and it has a transition to state 6, priority 1
			try { testTransition = testFsm.fsmTransition(5, 6, 1); }
			catch (...) { Assert::Fail(); }

			// and it is an atTarget
			if (dynamic_cast<jw::carFsm::atTarget*>(testTransition) == nullptr) Assert::Fail();

			// ###############
			// ### state 6 ###
			// ###############

			// check exists
			try { testState = testFsm.fsmState(6); }
			catch (...) { Assert::Fail(); }

			// and is an updatePath
			if (dynamic_cast<jw::carFsm::updatePath*>(testState) == nullptr) Assert::Fail();

			// and it has a transition to state 7, priority 1
			try { testTransition = testFsm.fsmTransition(6, 7, 1); }
			catch (...) { Assert::Fail(); }

			// and it is an arrived
			if (dynamic_cast<jw::carFsm::arrived*>(testTransition) == nullptr) Assert::Fail();

			// and it has a transition to state 4, priority 2
			try { testTransition = testFsm.fsmTransition(6, 4, 2); }
			catch (...) { Assert::Fail(); }

			// and it is a nullTransition
			if (dynamic_cast<jw::nullTransition*>(testTransition) == nullptr) Assert::Fail();

			// ###############
			// ### state 7 ###
			// ###############

			// check exists
			try { testState = testFsm.fsmState(7); }
			catch (...) { Assert::Fail(); }

			// and it is a pathToHome
			if (dynamic_cast<jw::carFsm::pathToHome*>(testState) == nullptr) Assert::Fail();

			// and it has a transition to state 8, priority 1
			try { testTransition = testFsm.fsmTransition(7, 8, 1); }
			catch (...) { Assert::Fail(); }

			// and it is a nullTransition
			if (dynamic_cast<jw::nullTransition*>(testTransition) == nullptr) Assert::Fail();

			// ###############
			// ### state 8 ###
			// ###############

			// check exists
			try { testState = testFsm.fsmState(8); }
			catch (...) { Assert::Fail(); }

			// and it is a targetRoadStart
			if (dynamic_cast<jw::carFsm::targetRoadStart*>(testState) == nullptr) Assert::Fail();

			// and it has a transition to state 9, priority 1
			try { testTransition = testFsm.fsmTransition(8, 9, 1); }
			catch (...) { Assert::Fail(); }

			// and it is an atTarget
			if (dynamic_cast<jw::carFsm::atTarget*>(testTransition) == nullptr) Assert::Fail();

			// ###############
			// ### state 9 ###
			// ###############

			// check exists
			try { testState = testFsm.fsmState(9); }
			catch (...) { Assert::Fail(); }

			// and it is a targetEndRoad
			if (dynamic_cast<jw::carFsm::targetRoadEnd*>(testState) == nullptr) Assert::Fail();

			// and it has a transition to state 10, priority 1
			try { testTransition = testFsm.fsmTransition(9, 10, 1); }
			catch (...) { Assert::Fail(); }

			// and it is an atTarget
			if (dynamic_cast<jw::carFsm::atTarget*>(testTransition) == nullptr) Assert::Fail();

			// ################
			// ### state 10 ###
			// ################

			// check exists
			try { testState = testFsm.fsmState(10); }
			catch (...) { Assert::Fail(); }

			// and it is a updatePath
			if (dynamic_cast<jw::carFsm::updatePath*>(testState) == nullptr) Assert::Fail();

			// and it has a transition to state 3, priority 1
			try { testTransition = testFsm.fsmTransition(10, 3, 1); }
			catch (...) { Assert::Fail(); }

			// and it is an arrived
			if (dynamic_cast<jw::carFsm::arrived*>(testTransition) == nullptr) Assert::Fail();

			// and it has a transition to state 8, priority 2
			try { testTransition = testFsm.fsmTransition(10, 8, 2); }
			catch (...) { Assert::Fail(); }

			// and it is a nullTransition
			if (dynamic_cast<jw::nullTransition*>(testTransition) == nullptr) Assert::Fail();

			// #############
			// ### other ###
			// #############

			// check initial state is 1
			Assert::IsTrue(testFsm.initialState() == 1);

			// check there is only 10 states
			Assert::IsTrue(testFsm.size() == 10);
		}
	};
}