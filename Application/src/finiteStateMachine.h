#pragma once

#include "graph.h"
#include "state.h"
#include "transition.h"
#include <SFML/System/Time.hpp>

namespace jw
{
	// Note: always initialise before update (especially after copy)
	class fsm
	{
	public:
		using state_type = state*;
		using transition_type = transition*;
		using container_type = graph<state_type, transition_type>;
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

		// Access
		// Added only for UTs
		state* getState(int stateId);	// TODO merge with setter
		state* currentState();
		transitions_container_type getTransitionsFromState(int stateId);
		transitions_container_type currentPossibleTransitions();
		int getInitialState();	// TODO merge with setter

		// Modifiers
		void addState(int id, state_type newNode);
		void addTransition(int fromId, int toId, transition_type newTransition);
		void setInitialState(int id);

	private:
		friend void swap(fsm& a, fsm& b);

		void checkTransitions();

		container_type fsmGraph;
		int initialStateId;
		state* _currentState;
		transitions_container_type* possibleTransitions;
	};
}