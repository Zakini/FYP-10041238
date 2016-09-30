#include "junctionController.h"

const int jw::junctionController::signalStateCount = 4;

jw::junctionController::junctionController(behaviour p_controllerBehaviour)
	: controllerBehaviour(p_controllerBehaviour)
	, junctionStates()
	, junctionCount()
	, lightSequence()
	, currentSequenceStep(lightSequence.end())
	, timeSinceLastLightChange()
	, lightChangeDelay(sf::seconds(5))
{
}

void jw::junctionController::update(sf::Time period)
{
	// TODO

	// check if it's time to change the lights
	timeSinceLastLightChange += period;
	if (timeSinceLastLightChange < lightChangeDelay) return;
	else timeSinceLastLightChange = sf::seconds(0);

	// check if the current sequence has finished
	bool sequenceFinished = true;

	for (int lightId : *currentSequenceStep)
	{
		if (junctionStates[lightId] != stop)
		{
			sequenceFinished = false;
			break;
		}
	}

	if (sequenceFinished)
	{
		// move to next sequence step
		currentSequenceStep++;
		if (currentSequenceStep == lightSequence.end()) currentSequenceStep = lightSequence.begin();
	}

	// update the lights
	for (int lightId : *currentSequenceStep)
	{
		junctionStates[lightId] = (signalState)((junctionStates[lightId] + 1) % signalStateCount);
	}
}

void jw::junctionController::setJunctionCount(int newCount)
{
	junctionCount = newCount;
	if (junctionCount > 0) buildSequence();

	for (int i = 0; i < junctionCount; i++)
	{
		junctionStates[i] = stop;
	}
}

const std::map<int, jw::junctionController::signalState>& jw::junctionController::getState()
{
	return junctionStates;
}

void jw::junctionController::buildSequence()
{
	lightSequence.clear();

	switch (controllerBehaviour)
	{
	case cycle:
		for (int i = 0; i < junctionCount; i++)
		{
			lightSequence.push_back({ i });
		}

		break;
	case none:
		// lights do not change
		break;
	}

	currentSequenceStep = lightSequence.end() - 1;
}