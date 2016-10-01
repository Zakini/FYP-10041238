#pragma once

#include <SFML/System/Time.hpp>
#include <map>
#include <vector>

namespace jw
{
	class junctionController
	{
	public:
		enum behaviour { none, cycle };
		enum signalState { stop, prepareToGo, go, stopIfAble };
		static const int signalStateCount;

		junctionController(behaviour p_controllerBehaviour = none);

		void update(sf::Time period);

		const std::map<int, signalState>& getState();
		int getJunctionCount() { return junctionCount; }
		sf::Time getLightChangeDelay() { return lightChangeDelay; }
		void setJunctionCount(int newCount);
		const std::vector<std::vector<int>>& getLightSequence() { return lightSequence; }

	private:
		behaviour controllerBehaviour;
		std::map<int, signalState> junctionStates;
		int junctionCount;
		std::vector<std::vector<int>> lightSequence;
		std::vector<std::vector<int>>::iterator currentSequenceStep;
		sf::Time timeSinceLastLightChange;
		sf::Time lightChangeDelay;

		void buildSequence();
	};
}