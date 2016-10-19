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
#include <memory>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(carFsm)
	{
	private:
		const string rootPath = "C:/Users/Josh Wells/Google Drive/Uni/Level 6/Final Year Project/Artefact/data/maps/tests";

	public:
		TEST_METHOD(moveToHome)
		{
			jw::world::graph_type* testGraph = jw::world::loadWorld(rootPath + "/carfsm-unit-test1.json");
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);

			auto pather = std::make_shared<jw::pathEngine>(testGraphSp);

			jw::car testCar(pather, nullptr, 1, 2, jw::fsm());
			jw::carFsm::moveToHome testState(testCar);

			testState.update(sf::seconds(0));	// move should be instantaneous

			Assert::IsTrue(testCar.getPosition() == pather->getLocationPosition(1));
			Assert::IsTrue(testCar.currentLocation() == 1);
		}

		TEST_METHOD(pathToHome)
		{
			jw::world::graph_type* testGraph = jw::world::loadWorld(rootPath + "/carfsm-unit-test1.json");
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);

			auto pather = std::make_shared<jw::pathEngine>(testGraphSp);

			jw::car testCar(pather, nullptr, 1, 2, jw::fsm());
			jw::carFsm::pathToHome testState(testCar);

			testCar.currentLocation(2);			// move to non-home location
			testState.update(sf::seconds(0));	// path generation is not dependant on world time

			std::deque<int> expectedPath;
			expectedPath.push_back(1);

			Assert::IsTrue(testCar.currentPath() == expectedPath);
		}

		TEST_METHOD(pathToWork)
		{
			jw::world::graph_type* testGraph = jw::world::loadWorld(rootPath + "/carfsm-unit-test1.json");
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);

			auto pather = std::make_shared<jw::pathEngine>(testGraphSp);

			jw::car testCar(pather, nullptr, 1, 2, jw::fsm());
			jw::carFsm::pathToWork testState(testCar);

			testCar.currentLocation(1);			// move to non-work location
			testState.update(sf::seconds(0));	// path generation is not dependant on world time

			std::deque<int> expectedPath;
			expectedPath.push_back(2);

			Assert::IsTrue(testCar.currentPath() == expectedPath);
		}
		
		TEST_METHOD(targetRoadStart)
		{
			jw::world::graph_type* testGraph = jw::world::loadWorld(rootPath + "/carfsm-unit-test2.json");
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);

			auto pather = std::make_shared<jw::pathEngine>(testGraphSp);

			jw::car testCar(pather, nullptr, 1, 2, jw::fsm());
			testCar.targetPosition(pather->getRoadEndPosition(1, 2));
			jw::carFsm::targetRoadStart testState(testCar);

			testCar.currentLocation(1);	// move to non-work location
			testCar.pathTo(2);			// generate path to work

			testState.update(sf::seconds(0));	// not dependent on world time

			Assert::IsTrue(testCar.targetPosition() == pather->getRoadStartPosition(1, 2));
		}

		TEST_METHOD(targetRoadEnd)
		{
			jw::world::graph_type* testGraph = jw::world::loadWorld(rootPath + "/carfsm-unit-test2.json");
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);

			auto pather = std::make_shared<jw::pathEngine>(testGraphSp);

			jw::car testCar(pather, nullptr, 1, 2, jw::fsm());
			testCar.targetPosition(pather->getRoadEndPosition(1, 2));
			jw::carFsm::targetRoadEnd testState(testCar);

			testCar.currentLocation(1);	// move to non-work location
			testCar.pathTo(2);			// generate path to work

			testState.update(sf::seconds(0));	// not dependent on world time

			Assert::IsTrue(testCar.targetPosition() == pather->getRoadEndPosition(1, 2));
		}

		TEST_METHOD(updatePath)
		{
			jw::world::graph_type* testGraph = jw::world::loadWorld(rootPath + "/carfsm-unit-test2.json");
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);

			auto pather = std::make_shared<jw::pathEngine>(testGraphSp);

			jw::car testCar(pather, nullptr, 1, 2, jw::fsm());
			testCar.currentLocation(1);
			testCar.pathTo(2);
			testCar.targetPosition(pather->getRoadEndPosition(1, 2));

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
			jw::world::graph_type* testGraph = jw::world::loadWorld(rootPath + "/carfsm-unit-test2.json");
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);

			auto pather = std::make_shared<jw::pathEngine>(testGraphSp);

			jw::car testCar(pather, nullptr, 1, 2, jw::fsm());
			testCar.currentLocation(1);
			testCar.targetPosition(pather->getRoadEndPosition(1, 2));
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
			jw::world::graph_type* testGraph = jw::world::loadWorld(rootPath + "/carfsm-unit-test2.json");
			std::shared_ptr<jw::pathEngine::graph_type> testGraphSp(testGraph);

			auto pather = std::make_shared<jw::pathEngine>(testGraphSp);

			jw::car testCar(pather, nullptr, 1, 2, jw::fsm());
			testCar.targetPosition(pather->getRoadEndPosition(1, 2));
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

		TEST_METHOD(lightAheadStop)
		{
			Assert::Fail();
		}

		TEST_METHOD(lightAheadStopIfAble)
		{
			Assert::Fail();
		}

		TEST_METHOD(lightAheadGo)
		{
			Assert::Fail();
		}

		TEST_METHOD(generate)
		{
			jw::car testCar(nullptr, nullptr, 1, 2);
			jw::fsm testFsm = jw::carFsm::generate(testCar);

			// ###############
			// ### state 1 ###
			// ###############

			Assert::IsTrue(checkStateType<jw::nullState>(testFsm, 1));
			Assert::IsTrue(checkTransitionType<jw::nullTransition>(testFsm, 1, 2, 1));

			// ###############
			// ### state 2 ###
			// ###############

			Assert::IsTrue(checkStateType<jw::carFsm::moveToHome>(testFsm, 2));
			Assert::IsTrue(checkTransitionType<jw::nullTransition>(testFsm, 2, 3, 1));

			// ###############
			// ### state 3 ###
			// ###############

			Assert::IsTrue(checkStateType<jw::carFsm::pathToWork>(testFsm, 3));
			Assert::IsTrue(checkTransitionType<jw::nullTransition>(testFsm, 3, 4, 1));

			// ###############
			// ### state 4 ###
			// ###############

			Assert::IsTrue(checkStateType<jw::carFsm::targetRoadStart>(testFsm, 4));
			Assert::IsTrue(checkTransitionType<jw::nullTransition>(testFsm, 4, 5, 1));

			// ###############
			// ### state 5 ###
			// ###############

			Assert::IsTrue(checkStateType<jw::nullState>(testFsm, 5));
			Assert::IsTrue(checkTransitionType<jw::carFsm::atTarget>(testFsm, 5, 6, 1));

			// ###############
			// ### state 6 ###
			// ###############

			Assert::IsTrue(checkStateType<jw::carFsm::targetRoadEnd>(testFsm, 6));
			Assert::IsTrue(checkTransitionType<jw::nullTransition>(testFsm, 6, 7, 1));

			// ###############
			// ### state 7 ###
			// ###############

			Assert::IsTrue(checkStateType<jw::nullState>(testFsm, 7));
			Assert::IsTrue(checkTransitionType<jw::carFsm::atTarget>(testFsm, 7, 8, 1));

			// ###############
			// ### state 8 ###
			// ###############

			Assert::IsTrue(checkStateType<jw::carFsm::updatePath>(testFsm, 8));
			Assert::IsTrue(checkTransitionType<jw::carFsm::arrived>(testFsm, 8, 9, 1));
			Assert::IsTrue(checkTransitionType<jw::nullTransition>(testFsm, 8, 4, 2));

			// ###############
			// ### state 9 ###
			// ###############

			Assert::IsTrue(checkStateType<jw::carFsm::pathToHome>(testFsm, 9));
			Assert::IsTrue(checkTransitionType<jw::nullTransition>(testFsm, 9, 10, 1));

			// ################
			// ### state 10 ###
			// ################

			Assert::IsTrue(checkStateType<jw::carFsm::targetRoadStart>(testFsm, 10));
			Assert::IsTrue(checkTransitionType<jw::nullTransition>(testFsm, 10, 11, 1));

			// ################
			// ### state 11 ###
			// ################

			Assert::IsTrue(checkStateType<jw::nullState>(testFsm, 11));
			Assert::IsTrue(checkTransitionType<jw::carFsm::atTarget>(testFsm, 11, 12, 1));

			// ################
			// ### state 12 ###
			// ################

			Assert::IsTrue(checkStateType<jw::carFsm::targetRoadEnd>(testFsm, 12));
			Assert::IsTrue(checkTransitionType<jw::nullTransition>(testFsm, 12, 13, 1));

			// ################
			// ### state 13 ###
			// ################

			Assert::IsTrue(checkStateType<jw::nullState>(testFsm, 13));
			Assert::IsTrue(checkTransitionType<jw::carFsm::atTarget>(testFsm, 13, 14, 1));

			// ################
			// ### state 14 ###
			// ################

			Assert::IsTrue(checkStateType<jw::carFsm::updatePath>(testFsm, 14));
			Assert::IsTrue(checkTransitionType<jw::carFsm::arrived>(testFsm, 14, 3, 1));
			Assert::IsTrue(checkTransitionType<jw::nullTransition>(testFsm, 14, 10, 2));

			// #############
			// ### other ###
			// #############

			// check initial state is 1
			Assert::IsTrue(testFsm.initialState() == 1);

			// check there is only 14 states
			Assert::IsTrue(testFsm.size() == 14);
		}
	
	private:
		template<typename ST>
		static bool checkStateType(jw::fsm& targetFsm, int stateToCheck)
		{
			jw::state* testState = targetFsm.fsmState(stateToCheck);

			return dynamic_cast<ST*>(testState) != nullptr;
		}

		template<typename TT>
		static bool checkTransitionType(jw::fsm& targetFsm, int transitionFrom, int transitionTo, int transitionPriority)
		{
			jw::transition* testTransition = targetFsm.fsmTransition(transitionFrom, transitionTo, transitionPriority);

			return dynamic_cast<TT*>(testTransition) != nullptr;
		}
	};
}