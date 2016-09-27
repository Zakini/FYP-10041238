#include "carFsm.h"
#include "car.h"
#include "vectorMaths.h"

#include <SFML/System/Vector2.hpp>
#include <stdexcept>

using jw::maths::length;
using std::out_of_range;

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

void jw::carFsm::targetRoadStart::update(sf::Time period)
{
	int targetLocationId = targetCar.currentPath().front();
	sf::Vector2f target = targetCar.pather()->getRoadStartPosition(targetCar.currentLocation(), targetLocationId);
	targetCar.targetPosition(target);
}

void jw::carFsm::targetRoadEnd::update(sf::Time period)
{
	int targetLocationId = targetCar.currentPath().front();
	sf::Vector2f target = targetCar.pather()->getRoadEndPosition(targetCar.currentLocation(), targetLocationId);
	targetCar.targetPosition(target);
}

void jw::carFsm::updatePath::update(sf::Time period)
{
	int targetLocationId = targetCar.currentPath().front();
	sf::Vector2f targetPosition = targetCar.pather()->getRoadEndPosition(targetCar.currentLocation(), targetLocationId);
	float distanceFromTarget = length(targetCar.position() - targetPosition);
	float currentSpeed = length(targetCar.velocity());
	if (distanceFromTarget <= arrivalDistanceThreshold && currentSpeed < arrivalSpeedThreshold)
	{
		targetCar.popStepFromPath();
	}
}

bool jw::carFsm::arrived::changeState()
{
	// if path is empty, we've popped off the target location, so we must have arrived!
	return targetCar.currentPath().empty();
}

bool jw::carFsm::atTarget::changeState()
{
	sf::Vector2f targetPosition;

	try { targetPosition = targetCar.targetPosition(); }
	catch (out_of_range oor) { return false; }

	float distanceFromTarget = length(targetCar.position() - targetPosition);
	float currentSpeed = length(targetCar.velocity());

	if (distanceFromTarget <= arrivalDistanceThreshold && currentSpeed < arrivalSpeedThreshold) return true;
	else return false;
}

jw::fsm jw::carFsm::generate(car& targetCar)
{
	fsm outputFsm;

	// TODO put nullStates after targetStates? no need to repeatedly set target
	outputFsm.fsmState( 1, new nullState());
	outputFsm.fsmState( 2, new moveToHome(targetCar));
	outputFsm.fsmState( 3, new pathToWork(targetCar));
	outputFsm.fsmState( 4, new targetRoadStart(targetCar));
	outputFsm.fsmState( 5, new targetRoadEnd(targetCar));
	outputFsm.fsmState( 6, new updatePath(targetCar));
	outputFsm.fsmState( 7, new pathToHome(targetCar));
	outputFsm.fsmState( 8, new targetRoadStart(targetCar));
	outputFsm.fsmState( 9, new targetRoadEnd(targetCar));
	outputFsm.fsmState(10, new updatePath(targetCar));

	outputFsm.fsmTransition( 1,  2, new nullTransition());
	outputFsm.fsmTransition( 2,  3, new nullTransition());
	outputFsm.fsmTransition( 3,  4, new nullTransition());
	outputFsm.fsmTransition( 4,  5, new atTarget(targetCar));
	outputFsm.fsmTransition( 5,  6, new atTarget(targetCar));
	outputFsm.fsmTransition( 6,  7, new arrived(targetCar));
	outputFsm.fsmTransition( 6,  4, new nullTransition());
	outputFsm.fsmTransition( 7,  8, new nullTransition());
	outputFsm.fsmTransition( 8,  9, new atTarget(targetCar));
	outputFsm.fsmTransition( 9, 10, new atTarget(targetCar));
	outputFsm.fsmTransition(10,  3, new arrived(targetCar));
	outputFsm.fsmTransition(10,  8, new nullTransition());

	outputFsm.initialState(1);

	return outputFsm;
}