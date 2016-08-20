#include "finiteStateMachine.h"

#include <stdexcept>
#include <utility>

using std::domain_error;

jw::fsm::fsm(const fsm& toCopy) : initialStateId(toCopy.initialStateId)
{
	container_type& thisGraph = this->fsmGraph;

	// copy states
	for (auto& idStatePair : toCopy.fsmGraph)
	{
		int currentID = idStatePair.first;
		state* currentState = idStatePair.second.first;

		thisGraph.insertNode(currentID, currentState->clone());
	}

	// copy transitions
	for (auto& idStatePair : toCopy.fsmGraph)
	{
		int currentFromId = idStatePair.first;
		const container_type::edge_container_type& currentTransitions = idStatePair.second.second;

		for (auto& idTransitionPair : currentTransitions)
		{
			int currentToId = idTransitionPair.first;
			transition* currentTransition = idTransitionPair.second;

			thisGraph.insertEdge(currentFromId, currentToId, currentTransition->clone());
		}
	}

	// TODO currentState and possibleTransitions
}

jw::fsm::fsm(fsm&& toMove) : fsm()
{
	swap(*this, toMove);
}

jw::fsm::~fsm()
{
	for (auto& idDataPair : fsmGraph)
	{
		auto& stateToDelete = idDataPair.second.first;
		auto& edgesToDelete = idDataPair.second.second;

		delete stateToDelete;

		for (auto& idTransPair : idDataPair.second.second)
		{
			delete idTransPair.second;
		}
	}
}

jw::fsm& jw::fsm::operator=(fsm assignFrom)
{
	swap(*this, assignFrom);
	return *this;
}

void jw::fsm::initialise()
{
	currentState = fsmGraph.nodeAt(initialStateId);
	possibleTransitions = &fsmGraph.edgesAt(initialStateId);
}

void jw::fsm::update(sf::Time period)
{
	if (currentState == nullptr) throw domain_error("fsm.update called before fsm.initialise");

	checkTransitions();
	currentState->update(period);
}

void jw::fsm::addState(int id, state_type newNode)
{
	fsmGraph.insertNode(id, newNode);
	setInitialState(id);
}

void jw::fsm::addTransition(int fromId, int toId, transition_type newTransition)
{
	fsmGraph.insertEdge(fromId, toId, newTransition);
}

void jw::fsm::setInitialState(int id)
{
	initialStateId = id;
}

// Note: update this function whenever member variables are added/removed!
void jw::swap(fsm& a, fsm& b)
{
	using std::swap;

	swap(a.fsmGraph, b.fsmGraph);
	swap(a.initialStateId, b.initialStateId);
	swap(a.currentState, b.currentState);
	swap(a.possibleTransitions, b.possibleTransitions);
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