#include "stdafx.h"
#include "CppUnitTest.h"
#include "../../Application/src/finiteStateMachine.h"
#include <SFML/System/Time.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(fsm)
	{
	public:
		TEST_METHOD(initialise)
		{
			jw::fsm testFsm;

			jw::state* expectedState = new jw::nullState();
			jw::transition* expectedTransition = new jw::nullTransition();
			testFsm.fsmState(1, expectedState);
			testFsm.fsmState(2, new jw::nullState());
			testFsm.fsmTransitions(1, 2, expectedTransition, 1);
			testFsm.initialState(1);

			testFsm.initialise();

			Assert::IsTrue(testFsm.currentState() == expectedState);
			Assert::IsTrue(testFsm.currentPossibleTransitions().at(std::make_pair(1, 2)) == expectedTransition);
		}

		TEST_METHOD(updateGetCurrent)
		{
			jw::fsm testFsm;

			jw::state* expectedState = new jw::nullState();
			testFsm.fsmState(1, new jw::nullState());
			testFsm.fsmState(2, expectedState);
			testFsm.fsmTransitions(1, 2, new jw::nullTransition());
			testFsm.initialState(1);

			testFsm.initialise();
			testFsm.update(sf::Time());	// zero time, null states/transitions do nothing anyway

			Assert::IsTrue(testFsm.currentState() == expectedState);
			Assert::IsTrue(testFsm.currentPossibleTransitions().empty());
		}

		TEST_METHOD(addGetState)
		{
			jw::fsm testFsm;

			jw::state* expectedState = new jw::nullState();
			testFsm.fsmState(1, expectedState);

			Assert::IsTrue(testFsm.fsmState(1) == expectedState);
		}

		TEST_METHOD(addGetTransition)
		{
			jw::fsm testFsm;

			jw::transition* expectedTransition = new jw::nullTransition();
			testFsm.fsmState(1, new jw::nullState());
			testFsm.fsmState(2, new jw::nullState());
			testFsm.fsmTransitions(1, 2, expectedTransition, 1);

			Assert::IsTrue(testFsm.fsmTransitions(1).at(std::make_pair(1, 2)) == expectedTransition);
			Assert::Fail();	// TODO test multiple priorities + auto priorities
		}

		TEST_METHOD(setGetInitialState)
		{
			jw::fsm testFsm;

			testFsm.initialState(1);

			Assert::IsTrue(testFsm.initialState() == 1);
		}
	};
}