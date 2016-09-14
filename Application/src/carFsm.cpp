#include "carFsm.h"
#include "car.h"
#include "vectorMaths.h"

#include <SFML/System/Vector2.hpp>

using jw::maths::length;

void jw::carFsm::moveToHome::update(sf::Time period)
{
	targetCar.currentLocation(targetCar.homeLocation());
}

void jw::carFsm::pathToHome::update(sf::Time period)
{
	targetCar.pathTo(targetCar.homeLocation());
}

void jw::carFsm::pathToWork::update(sf::Time period)
{
	targetCar.pathTo(targetCar.workLocation());
}

void jw::carFsm::travelling::update(sf::Time period)
{
	targetCar.generateForce(period);

	// check if reached current step of path
	int targetLocationId = targetCar._currentPath.front();
	sf::Vector2f targetPosition = targetCar.pather->getLocationPosition(targetLocationId);
	float distanceFromTarget = length(targetCar._position - targetPosition);
	float currentSpeed = length(targetCar.velocity);
	if (distanceFromTarget <= arrivalThreshold && currentSpeed < 1)
	{
		targetCar.currentLocationID = targetLocationId;
		targetCar._currentPath.pop_front();
	}
}

bool jw::carFsm::arrived::changeState()
{
	// if path is empty, we've popped off the target location, so we must have arrived!
	return targetCar._currentPath.empty();
}

jw::fsm jw::carFsm::generate(car& targetCar)
{
	fsm outputFsm;

	outputFsm.fsmState(1, new nullState());
	outputFsm.fsmState(2, new moveToHome(targetCar));
	outputFsm.fsmState(3, new pathToWork(targetCar));
	outputFsm.fsmState(4, new travelling(targetCar));
	outputFsm.fsmState(5, new pathToHome(targetCar));
	outputFsm.fsmState(6, new travelling(targetCar));

	outputFsm.fsmTransitions(1, 2, new nullTransition());
	outputFsm.fsmTransitions(2, 3, new nullTransition());
	outputFsm.fsmTransitions(3, 4, new nullTransition());
	outputFsm.fsmTransitions(4, 5, new arrived(targetCar));
	outputFsm.fsmTransitions(5, 6, new nullTransition());
	outputFsm.fsmTransitions(6, 3, new arrived(targetCar));

	outputFsm.initialState(1);

	return outputFsm;
}