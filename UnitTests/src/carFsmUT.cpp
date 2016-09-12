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

			jw::car testCar(&pather, 1, 2);
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

			jw::car testCar(&pather, 1, 2);
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

			jw::car testCar(&pather, 1, 2);
			jw::carFsm::pathToWork testState(testCar);

			testCar.currentLocation(1);			// move to non-work location
			testState.update(sf::seconds(0));	// path generation is not dependant on world time

			std::deque<int> expectedPath;
			expectedPath.push_back(2);

			Assert::IsTrue(testCar.currentPath() == expectedPath);
		}

		TEST_METHOD(travelling)
		{
			string worldJsonFilePath = "C:/Users/Josh Wells/Google Drive/Uni/Level 6/Final Year Project/Artefact/data/maps/carfsm-unit-test2.json";
			jw::world::graph_type* testGraph = jw::world::loadWorld(worldJsonFilePath);
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);

			jw::pathEngine pather(testGraphSp);

			jw::car testCar(&pather, 1, 2);
			jw::carFsm::travelling testState(testCar);

			testCar.currentLocation(1);	// move to non-work location
			testCar.pathTo(2);			// generate path to work

			sf::Clock timer;	// start

			while (!testCar.currentPath().empty())
			{
				// repeated update travelling state until we arrive at the location
				testState.update(sf::milliseconds(10));	// ~60fps

				if (timer.getElapsedTime().asSeconds() >= 10)	// 10 sec time limit
				{
					Assert::Fail();
				}
			}

			Assert::IsTrue(true);	// pass
		}

		TEST_METHOD(arrived)
		{
			string worldJsonFilePath = "C:/Users/Josh Wells/Google Drive/Uni/Level 6/Final Year Project/Artefact/data/maps/carfsm-unit-test2.json";
			jw::world::graph_type* testGraph = jw::world::loadWorld(worldJsonFilePath);
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);

			jw::pathEngine pather(testGraphSp);

			jw::car testCar(&pather, 1, 2);
			jw::carFsm::travelling setupState(testCar);
			jw::carFsm::arrived testState(testCar);

			testCar.currentLocation(1);	// move to non-work location
			testCar.pathTo(2);			// generate path to work

			sf::Clock timer;	// start

			while (!testCar.currentPath().empty())
			{
				// repeated update travelling state until we arrive at the location
				setupState.update(sf::milliseconds(10));	// ~60fps

				if (timer.getElapsedTime().asSeconds() >= 10)	// 10 sec time limit
				{
					Assert::Fail();	// this is travelling failing, not arrived
				}
			}

			Assert::IsTrue(testState.changeState());
		}

		TEST_METHOD(generate)
		{
			// TODO dynamic casts don't throw
			jw::car testCar(nullptr, 1, 2);
			jw::fsm testFsm = jw::carFsm::generate(testCar);
			jw::fsm::state_type testState;
			jw::fsm::transitions_container_type testTransitions;

			// state 1

			// check exists
			try { testState = testFsm.getState(1); }
			catch (...)	{ Assert::Fail(); }

			// and is a nullState
			if (dynamic_cast<jw::nullState*>(testState) == nullptr) Assert::Fail();

			// and it has a transition to state 2
			testTransitions = testFsm.getTransitionsFromState(1);

			try { testTransitions.at(2); }
			catch (...) { Assert::Fail(); }

			// state 2

			// check exists
			try { testState = testFsm.getState(2); }
			catch (...) { Assert::Fail(); }

			// and is a moveToHome
			if (dynamic_cast<jw::carFsm::moveToHome*>(testState) == nullptr) Assert::Fail();

			// and it has a transition to state 3
			testTransitions = testFsm.getTransitionsFromState(2);

			try { testTransitions.at(3); }
			catch (...) { Assert::Fail(); }

			// state 3

			// check exists
			try { testState = testFsm.getState(3); }
			catch (...) { Assert::Fail(); }

			// and is a pathToWork
			if (dynamic_cast<jw::carFsm::pathToWork*>(testState) == nullptr) Assert::Fail();

			// and it has a transition to state 4
			testTransitions = testFsm.getTransitionsFromState(3);

			try { testTransitions.at(4); }
			catch (...) { Assert::Fail(); }

			// state 4

			// check exists
			try { testState = testFsm.getState(4); }
			catch (...) { Assert::Fail(); }

			// and is a travelling
			if (dynamic_cast<jw::carFsm::travelling*>(testState) == nullptr) Assert::Fail();

			// and it has a transition to state 5
			testTransitions = testFsm.getTransitionsFromState(4);

			try { testTransitions.at(5); }
			catch (...) { Assert::Fail(); }

			// state 5

			// check exists
			try { testState = testFsm.getState(5); }
			catch (...) { Assert::Fail(); }

			// and is a pathToHome
			if (dynamic_cast<jw::carFsm::pathToHome*>(testState) == nullptr) Assert::Fail();

			// and it has a transition to state 6
			testTransitions = testFsm.getTransitionsFromState(5);

			try { testTransitions.at(6); }
			catch (...) { Assert::Fail(); }

			// state 6

			// check exists
			try { testState = testFsm.getState(6); }
			catch (...) { Assert::Fail(); }

			// and is a travelling
			if (dynamic_cast<jw::carFsm::travelling*>(testState) == nullptr) Assert::Fail();

			// and it has a transition to state 3
			testTransitions = testFsm.getTransitionsFromState(6);

			try { testTransitions.at(3); }
			catch (...) { Assert::Fail(); }

			// check initial state is 1
			Assert::IsTrue(testFsm.getInitialState() == 1);
		}
	};
}