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
	if (targetCar.isAtTarget())
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
	return targetCar.isAtTarget();
}

jw::fsm jw::carFsm::generate(car& targetCar)
{
	fsm outputFsm;

	outputFsm.fsmState( 1, new nullState());
	outputFsm.fsmState( 2, new moveToHome(targetCar));
	outputFsm.fsmState( 3, new pathToWork(targetCar));
	outputFsm.fsmState( 4, new targetRoadStart(targetCar));
	outputFsm.fsmState( 5, new nullState());
	outputFsm.fsmState( 6, new targetRoadEnd(targetCar));
	outputFsm.fsmState( 7, new nullState());
	outputFsm.fsmState( 8, new updatePath(targetCar));
	outputFsm.fsmState( 9, new pathToHome(targetCar));
	outputFsm.fsmState(10, new targetRoadStart(targetCar));
	outputFsm.fsmState(11, new nullState());
	outputFsm.fsmState(12, new targetRoadEnd(targetCar));
	outputFsm.fsmState(13, new nullState());
	outputFsm.fsmState(14, new updatePath(targetCar));

	outputFsm.fsmTransition( 1,  2, new nullTransition());
	outputFsm.fsmTransition( 2,  3, new nullTransition());
	outputFsm.fsmTransition( 3,  4, new nullTransition());
	outputFsm.fsmTransition( 4,  5, new nullTransition());
	outputFsm.fsmTransition( 5,  6, new atTarget(targetCar));
	outputFsm.fsmTransition( 6,  7, new nullTransition());
	outputFsm.fsmTransition( 7,  8, new atTarget(targetCar));
	outputFsm.fsmTransition( 8,  9, new arrived(targetCar));
	outputFsm.fsmTransition( 8,  4, new nullTransition());
	outputFsm.fsmTransition( 9, 10, new nullTransition());
	outputFsm.fsmTransition(10, 11, new nullTransition());
	outputFsm.fsmTransition(11, 12, new atTarget(targetCar));
	outputFsm.fsmTransition(12, 13, new nullTransition());
	outputFsm.fsmTransition(13, 14, new atTarget(targetCar));
	outputFsm.fsmTransition(14,  3, new arrived(targetCar));
	outputFsm.fsmTransition(14, 10, new nullTransition());

	outputFsm.initialState(1);

	return outputFsm;
}