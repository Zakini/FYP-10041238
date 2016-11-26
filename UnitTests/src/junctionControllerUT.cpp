#include "stdafx.h"
#include "CppUnitTest.h"
#include "../../Application/src/junctionController.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(junctionController)
	{
		TEST_METHOD(getSetJunctionCount)
		{
			jw::junctionController testController;

			Assert::IsTrue(testController.getJunctionCount() == 0);

			testController.setJunctionCount(5);
			Assert::IsTrue(testController.getJunctionCount() == 5);
		}

		TEST_METHOD(getLightSequence)
		{
			jw::junctionController testController(jw::junctionController::behaviour::cycle);
			testController.setJunctionCount(3);

			std::vector<std::vector<int>> expectedSequence = { {0}, {1}, {2} };

			Assert::IsTrue(testController.getLightSequence() == expectedSequence);
		}

		TEST_METHOD(getLightChangeDelay)
		{
			jw::junctionController testController;

			Assert::IsTrue(testController.getLightChangeDelay() == sf::seconds(5));
		}

		TEST_METHOD(getControllerBehaviour)
		{
			jw::junctionController testController;

			Assert::IsTrue(testController.getControllerBehaviour() == jw::junctionController::behaviour::none);

			testController = jw::junctionController(jw::junctionController::behaviour::cycle);

			Assert::IsTrue(testController.getControllerBehaviour() == jw::junctionController::behaviour::cycle);
		}

		TEST_METHOD(update)
		{
			// controller behaviour: none
			jw::junctionController testController;
			testController.setJunctionCount(3);

			sf::Time delay = testController.getLightChangeDelay();

			std::map<int, jw::junctionController::signalState> expectedState = {
				{ 0, jw::junctionController::signalState::stop }
			,	{ 1, jw::junctionController::signalState::stop }
			,	{ 2, jw::junctionController::signalState::stop }
			};

			for (int i = 0; i < 1000; i++)
			{
				testController.update(delay);
				Assert::IsTrue(testController.getState() == expectedState);	// state should never change
			}

			// controller behaviour: cycle
			testController = jw::junctionController(jw::junctionController::behaviour::cycle);
			testController.setJunctionCount(3);

			std::vector<std::map<int, jw::junctionController::signalState>> expectedStates = {
				{
					{ 0, jw::junctionController::signalState::stop }
				,	{ 1, jw::junctionController::signalState::stop }
				,	{ 2, jw::junctionController::signalState::stop }
				}
			,	{
					{ 0, jw::junctionController::signalState::prepareToGo }
				,	{ 1, jw::junctionController::signalState::stop }
				,	{ 2, jw::junctionController::signalState::stop }
				}
			,	{
					{ 0, jw::junctionController::signalState::go }
				,	{ 1, jw::junctionController::signalState::stop }
				,	{ 2, jw::junctionController::signalState::stop }
				}
			,	{
					{ 0, jw::junctionController::signalState::stopIfAble }
				,	{ 1, jw::junctionController::signalState::stop }
				,	{ 2, jw::junctionController::signalState::stop }
				}
			,	{
					{ 0, jw::junctionController::signalState::stop }
				,	{ 1, jw::junctionController::signalState::stop }
				,	{ 2, jw::junctionController::signalState::stop }
				}
			,	{
					{ 0, jw::junctionController::signalState::stop }
				,	{ 1, jw::junctionController::signalState::prepareToGo }
				,	{ 2, jw::junctionController::signalState::stop }
				}
			,	{
					{ 0, jw::junctionController::signalState::stop }
				,	{ 1, jw::junctionController::signalState::go }
				,	{ 2, jw::junctionController::signalState::stop }
				}
			,	{
					{ 0, jw::junctionController::signalState::stop }
				,	{ 1, jw::junctionController::signalState::stopIfAble }
				,	{ 2, jw::junctionController::signalState::stop }
				}
			,	{
					{ 0, jw::junctionController::signalState::stop }
				,	{ 1, jw::junctionController::signalState::stop }
				,	{ 2, jw::junctionController::signalState::stop }
				}
			,	{
					{ 0, jw::junctionController::signalState::stop }
				,	{ 1, jw::junctionController::signalState::stop }
				,	{ 2, jw::junctionController::signalState::prepareToGo }
				}
			,	{
					{ 0, jw::junctionController::signalState::stop }
				,	{ 1, jw::junctionController::signalState::stop }
				,	{ 2, jw::junctionController::signalState::go }
				}
			,	{
					{ 0, jw::junctionController::signalState::stop }
				,	{ 1, jw::junctionController::signalState::stop }
				,	{ 2, jw::junctionController::signalState::stopIfAble }
				}
			};
			
			for (auto expectedState : expectedStates)
			{
				Assert::IsTrue(testController.getState() == expectedState);
				testController.update(delay);
			}

			// check state loops around to beginning again
			Assert::IsTrue(testController.getState() == expectedStates[0]);
		}
	};
}