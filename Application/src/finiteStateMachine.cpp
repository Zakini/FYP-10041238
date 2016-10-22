#include "finiteStateMachine.h"

#include <stdexcept>

using std::domain_error;
using std::make_pair;

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
			int currentToId = idTransitionPair.first.second;
			int currentEdgePriority = idTransitionPair.first.first;
			transition* currentTransition = idTransitionPair.second;

			thisGraph.insertEdge(currentFromId, currentToId, currentTransition->clone(), currentEdgePriority);
		}
	}

	// copy other bits
	this->currentStateId = toCopy.currentStateId;

	if (toCopy._currentState != nullptr)
	{
		this->_currentState = this->fsmGraph.nodeAt(this->currentStateId);
		this->possibleTransitions = &this->fsmGraph.edgesAt(this->currentStateId);
	}
	else
	{
		this->_currentState = nullptr;
		this->possibleTransitions = nullptr;
	}
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
	if (fsmGraph.empty()) return;

	currentStateId = initialStateId;
	_currentState = fsmGraph.nodeAt(initialStateId);
	possibleTransitions = &fsmGraph.edgesAt(initialStateId);
}

void jw::fsm::update(sf::Time period)
{
	if (fsmGraph.empty()) return;
	if (_currentState == nullptr) throw domain_error("fsm.update called before fsm.initialise");

	checkTransitions();
	_currentState->update(period);
}

jw::state* jw::fsm::fsmState(int stateId)
{
	return fsmGraph.nodeAt(stateId);	// POSSIBLE clone?
}

jw::state* jw::fsm::currentState()
{
	return _currentState;
}

jw::fsm::transition_type jw::fsm::fsmTransition(int fromId, int toId, int transitionPriority)
{
	return fsmGraph.edgeBetween(fromId, toId, transitionPriority);
}

jw::fsm::transitions_container_type jw::fsm::currentPossibleTransitions()
{
	return *possibleTransitions;
}

int jw::fsm::initialState()
{
	return initialStateId;
}

void jw::fsm::fsmState(int id, state_type newNode)
{
	fsmGraph.insertNode(id, newNode);
	initialState(id);
}

void jw::fsm::fsmTransition(int fromId, int toId, transition_type newTransition)
{
	fsmGraph.insertEdge(fromId, toId, newTransition);
}

void jw::fsm::fsmTransition(int fromId, int toId, transition_type newTransition, int priority)
{
	fsmGraph.insertEdge(fromId, toId, newTransition, priority);
}

void jw::fsm::initialState(int id)
{
	initialStateId = id;
}

// Note: update this function whenever member variables are added/removed!
void jw::swap(fsm& a, fsm& b)
{
	using std::swap;

	swap(a.fsmGraph, b.fsmGraph);
	swap(a.initialStateId, b.initialStateId);
	swap(a._currentState, b._currentState);
	swap(a.possibleTransitions, b.possibleTransitions);
}

void jw::fsm::checkTransitions()
{
	for (auto& currentTransitionPair : *possibleTransitions)
	{
		int possibleNewState = currentTransitionPair.first.second;
		transition* currentTransition = currentTransitionPair.second;

		if (currentTransition->changeState())
		{
			currentStateId = possibleNewState;
			_currentState = fsmGraph.nodeAt(possibleNewState);
			possibleTransitions = &fsmGraph.edgesAt(possibleNewState);
			break;
		}
	}
}