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
			testFsm.addState(1, expectedState);
			testFsm.addState(2, new jw::nullState());
			testFsm.addTransition(1, 2, expectedTransition);
			testFsm.setInitialState(1);

			testFsm.initialise();

			Assert::IsTrue(testFsm.currentState() == expectedState);
			Assert::IsTrue(testFsm.currentPossibleTransitions().at(2) == expectedTransition);
		}

		TEST_METHOD(update)
		{
			jw::fsm testFsm;

			jw::state* expectedState = new jw::nullState();
			testFsm.addState(1, new jw::nullState());
			testFsm.addState(2, expectedState);
			testFsm.addTransition(1, 2, new jw::nullTransition());
			testFsm.setInitialState(1);

			testFsm.initialise();
			testFsm.update(sf::Time());	// zero time, null states/transitions do nothing anyway

			Assert::IsTrue(testFsm.currentState() == expectedState);
			Assert::IsTrue(testFsm.currentPossibleTransitions().empty());
		}

		TEST_METHOD(addGetState)
		{
			jw::fsm testFsm;

			jw::state* expectedState = new jw::nullState();
			testFsm.addState(1, expectedState);

			Assert::IsTrue(testFsm.getState(1) == expectedState);
		}

		TEST_METHOD(addGetTransition)
		{
			jw::fsm testFsm;

			jw::transition* expectedTransition = new jw::nullTransition();
			testFsm.addState(1, new jw::nullState());
			testFsm.addState(2, new jw::nullState());
			testFsm.addTransition(1, 2, expectedTransition);

			Assert::IsTrue(testFsm.getTransitionsFromState(1).at(2) == expectedTransition);
		}

		TEST_METHOD(setGetInitialState)
		{
			jw::fsm testFsm;

			testFsm.setInitialState(1);

			Assert::IsTrue(testFsm.getInitialState() == 1);
		}
	};
}