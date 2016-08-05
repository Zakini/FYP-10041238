#include "carFsm.h"

void jw::carFsm::travelling::update()
{
	// TODO
}

bool jw::carFsm::arrived::changeState()
{
	if (false /* TODO car is at target location */)
	{
		// TODO generate new path etc.
		return true;
	}
	else
	{
		return false;
	}
}

jw::fsm jw::carFsm::generate(car& targetCar)
{
	fsm outputFsm;

	outputFsm.addState(1, new carFsm::travelling(targetCar));
	outputFsm.addTransition(1, 1, new carFsm::arrived(targetCar));

	return outputFsm;
}