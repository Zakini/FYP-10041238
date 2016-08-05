#pragma once

#include "graph.h"
#include "state.h"
#include "transition.h"

namespace jw
{
	// Note: always initialise before update (especially after copy)
	class fsm
	{
	public:
		using state_type = state*;
		using transition_type = transition*;
		using container_type = graph<state_type, transition_type>;

		// Usage
		void initialise();
		void update();

		// Modifiers
		bool addState(int id, state_type newNode);
		bool addTransition(int fromId, int toId, transition_type newTransition);
		void setInitialState(int id);

	private:
		void checkTransitions();

		container_type fsmGraph;
		int initialStateId;
		state* currentState;
		container_type::edge_container_type* possibleTransitions;
	};
}