#include "finiteStateMachine.h"

void jw::fsm::initialise()
{
	currentState = fsmGraph.nodeAt(initialStateId);
	possibleTransitions = &fsmGraph.edgesAt(initialStateId);
}

void jw::fsm::update()
{
	checkTransitions();
	currentState->update();
}

bool jw::fsm::addState(int id, state_type newNode)
{
	bool insertResult = fsmGraph.insertNode(id, newNode);

	if (insertResult) setInitialState(id);

	return insertResult;
}

bool jw::fsm::addTransition(int fromId, int toId, transition_type newTransition)
{
	return fsmGraph.insertEdge(fromId, toId, newTransition);
}

void jw::fsm::setInitialState(int id)
{
	initialStateId = id;
}

void jw::fsm::checkTransitions()
{
	for (auto& currentTransitionPair : *possibleTransitions)
	{
		int possibleNewState = currentTransitionPair.first;
		transition* currentTransition = currentTransitionPair.second;

		if (currentTransition->changeState())
		{
			currentState = fsmGraph.nodeAt(possibleNewState);
			possibleTransitions = &fsmGraph.edgesAt(possibleNewState);
			break;
		}
	}
}