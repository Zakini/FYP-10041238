#include "carFsm.h"
#include "car.h"

#include <SFML/System/Vector2.hpp>

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

	// TODO move to arrived state?
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

	outputFsm.addState(1, new nullState());
	outputFsm.addState(2, new moveToHome(targetCar));
	outputFsm.addState(3, new pathToWork(targetCar));
	outputFsm.addState(4, new travelling(targetCar));
	outputFsm.addState(5, new pathToHome(targetCar));
	outputFsm.addState(6, new travelling(targetCar));

	outputFsm.addTransition(1, 2, new nullTransition());
	outputFsm.addTransition(2, 3, new nullTransition());
	outputFsm.addTransition(3, 4, new nullTransition());
	outputFsm.addTransition(4, 5, new arrived(targetCar));
	outputFsm.addTransition(5, 6, new nullTransition());
	outputFsm.addTransition(6, 3, new arrived(targetCar));

	outputFsm.setInitialState(1);

	return outputFsm;
}