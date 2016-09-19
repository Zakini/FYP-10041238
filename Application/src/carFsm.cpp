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

void jw::carFsm::enterRoad::update(sf::Time period)
{
	int targetLocationId = targetCar._currentPath.front();
	sf::Vector2f targetPosition = targetCar.pather->getRoadStartPosition(targetCar.currentLocationID, targetLocationId);
	sf::Vector2f force = targetCar.generateForce(targetPosition, period);
	targetCar.applyForce(force, period);
}

void jw::carFsm::followRoad::update(sf::Time period)
{
	int targetLocationId = targetCar._currentPath.front();
	sf::Vector2f targetPosition = targetCar.pather->getRoadEndPosition(targetCar.currentLocationID, targetLocationId);
	sf::Vector2f force = targetCar.generateForce(targetPosition, period);
	targetCar.applyForce(force, period);
}

void jw::carFsm::updatePath::update(sf::Time period)
{
	int targetLocationId = targetCar._currentPath.front();
	sf::Vector2f targetPosition = targetCar.pather->getRoadEndPosition(targetCar.currentLocationID, targetLocationId);
	float distanceFromTarget = length(targetCar._position - targetPosition);
	float currentSpeed = length(targetCar.velocity);
	if (distanceFromTarget <= arrivalDistanceThreshold && currentSpeed < arrivalSpeedThreshold)
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

bool jw::carFsm::atRoadStart::changeState()
{
	int targetLocationId = targetCar._currentPath.front();
	sf::Vector2f targetPosition = targetCar.pather->getRoadStartPosition(targetCar.currentLocationID, targetLocationId);

	float distanceFromTarget = length(targetCar._position - targetPosition);
	float currentSpeed = length(targetCar.velocity);

	if (distanceFromTarget <= arrivalDistanceThreshold && currentSpeed < arrivalSpeedThreshold) return true;
	else return false;
}

bool jw::carFsm::atRoadEnd::changeState()
{
	int targetLocationId = targetCar._currentPath.front();
	sf::Vector2f targetPosition = targetCar.pather->getRoadEndPosition(targetCar.currentLocationID, targetLocationId);

	float distanceFromTarget = length(targetCar._position - targetPosition);
	float currentSpeed = length(targetCar.velocity);

	if (distanceFromTarget <= arrivalDistanceThreshold && currentSpeed < arrivalSpeedThreshold) return true;
	else return false;
}

jw::fsm jw::carFsm::generate(car& targetCar)
{
	fsm outputFsm;

	outputFsm.fsmState( 1, new nullState());
	outputFsm.fsmState( 2, new moveToHome(targetCar));
	outputFsm.fsmState( 3, new pathToWork(targetCar));
	outputFsm.fsmState( 4, new enterRoad(targetCar));
	outputFsm.fsmState( 5, new followRoad(targetCar));
	outputFsm.fsmState( 6, new updatePath(targetCar));
	outputFsm.fsmState( 7, new pathToHome(targetCar));
	outputFsm.fsmState( 8, new enterRoad(targetCar));
	outputFsm.fsmState( 9, new followRoad(targetCar));
	outputFsm.fsmState(10, new updatePath(targetCar));

	outputFsm.fsmTransitions( 1,  2, new nullTransition());
	outputFsm.fsmTransitions( 2,  3, new nullTransition());
	outputFsm.fsmTransitions( 3,  4, new nullTransition());
	outputFsm.fsmTransitions( 4,  5, new atRoadStart(targetCar));
	outputFsm.fsmTransitions( 5,  6, new atRoadEnd(targetCar));
	outputFsm.fsmTransitions( 6,  7, new arrived(targetCar));
	outputFsm.fsmTransitions( 6,  4, new nullTransition());
	outputFsm.fsmTransitions( 7,  8, new nullTransition());
	outputFsm.fsmTransitions( 8,  9, new atRoadStart(targetCar));
	outputFsm.fsmTransitions( 9, 10, new atRoadEnd(targetCar));
	outputFsm.fsmTransitions(10,  3, new arrived(targetCar));
	outputFsm.fsmTransitions(10,  8, new nullTransition());

	outputFsm.initialState(1);

	return outputFsm;
}