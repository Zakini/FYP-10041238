#pragma once

#include "priorityGraph.h"
#include "state.h"
#include "transition.h"
#include <SFML/System/Time.hpp>
#include <utility>

using std::pair;

namespace jw
{
	// Note: always initialise before update (especially after copy)
	class fsm
	{
	public:
		using state_type = state*;
		using transition_type = transition*;
		using transition_key = pair<int, int>;	// <priority/order, target id>
		using container_type = priorityGraph<state_type, transition_type>;
		using transitions_container_type = container_type::edge_container_type;

		// POSSIBLE UT c'tors/d'tors/assign op?
		// Copy and swap idiom: http://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom
		fsm() = default;
		fsm(const fsm& toCopy);
		fsm(fsm&& toMove);
		~fsm();

		fsm& operator=(fsm assignFrom);

		// Usage
		void initialise();
		void update(sf::Time period);

		// Meta
		int size() { return fsmGraph.size(); }

		// Access
		// Added only for UTs
		state_type fsmState(int stateId);
		state_type currentState();
		transition_type fsmTransition(int fromId, int toId, int transitionPriority);
		transitions_container_type currentPossibleTransitions();
		int initialState();

		// Modifiers
		void fsmState(int id, state_type newNode);
		void fsmTransition(int fromId, int toId, transition_type newTransition);
		void fsmTransition(int fromId, int toId, transition_type newTransition, int priority);
		void initialState(int id);

	private:
		friend void swap(fsm& a, fsm& b);

		void checkTransitions();

		container_type fsmGraph;
		int initialStateId, currentStateId;
		state* _currentState;
		transitions_container_type* possibleTransitions;
	};
}