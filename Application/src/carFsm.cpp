#include "carFsm.h"
#include "car.h"
#include "vectorMaths.h"

#include <SFML/System/Vector2.hpp>
#include <stdexcept>

using jw::maths::length;
using std::out_of_range;

using signalState = jw::junctionController::signalState;

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
	if (targetCar.isAtTarget())	// TODO remove?
	{
		targetCar.completePathStep();
	}
}

void jw::carFsm::moveToTarget::update(sf::Time period)
{
	targetCar.moveTowardTarget(period);
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

bool jw::carFsm::lightAheadStop::changeState()
{
	pair<const sf::Vector2f*, const junctionController::signalState*> trafficLightState = targetCar.getTrafficLightPerception();

	if (trafficLightState.first == nullptr || trafficLightState.second == nullptr) return false;	// no light, no need to stop

	return *trafficLightState.second == signalState::stop;
}

bool jw::carFsm::lightAheadAbleToStop::changeState()
{
	pair<const sf::Vector2f*, const junctionController::signalState*> trafficLightState = targetCar.getTrafficLightPerception();
	
	if (trafficLightState.first == nullptr || trafficLightState.second == nullptr) return false;	// no light, no need to stop

	// TODO check cars behind
	return *trafficLightState.second == signalState::stopIfAble;
}

bool jw::carFsm::lightAheadGo::changeState()
{
	pair<const sf::Vector2f*, const junctionController::signalState*> trafficLightState = targetCar.getTrafficLightPerception();
	
	if (trafficLightState.first == nullptr || trafficLightState.second == nullptr) return true;	// no light, go ahead

	return *trafficLightState.second == signalState::go || *trafficLightState.second == signalState::prepareToGo;
}

bool jw::carFsm::targetClear::changeState()
{
	return targetCar.isTargetClear();
}

jw::fsm jw::carFsm::generate(car& targetCar)
{
	fsm outputFsm;
	// TODO reuse elements?
	outputFsm.fsmState( 1, new nullState());	// start
	outputFsm.fsmState( 2, new moveToHome(targetCar));
	outputFsm.fsmState( 3, new pathToWork(targetCar));
	outputFsm.fsmState( 4, new targetRoadStart(targetCar));
	outputFsm.fsmState( 5, new moveToTarget(targetCar));	// travelling
	outputFsm.fsmState( 6, new targetRoadEnd(targetCar));
	outputFsm.fsmState( 7, new moveToTarget(targetCar));	// waiting at traffic light
	outputFsm.fsmState( 8, new moveToTarget(targetCar));	// moving through traffic light
	outputFsm.fsmState( 9, new updatePath(targetCar));
	outputFsm.fsmState(10, new pathToHome(targetCar));
	outputFsm.fsmState(11, new targetRoadStart(targetCar));
	outputFsm.fsmState(12, new moveToTarget(targetCar));	// travelling
	outputFsm.fsmState(13, new targetRoadEnd(targetCar));
	outputFsm.fsmState(14, new moveToTarget(targetCar));	// waiting at traffic light
	outputFsm.fsmState(15, new moveToTarget(targetCar));	// moving through traffic light
	outputFsm.fsmState(16, new updatePath(targetCar));

	outputFsm.fsmTransition( 1,  2, new nullTransition());
	outputFsm.fsmTransition( 2,  3, new nullTransition());
	outputFsm.fsmTransition( 3,  4, new nullTransition());
	outputFsm.fsmTransition( 4,  5, new targetClear(targetCar));
	outputFsm.fsmTransition( 5,  6, new atTarget(targetCar));
	outputFsm.fsmTransition( 6,  7, new lightAheadStop(targetCar));
	outputFsm.fsmTransition( 6,  7, new lightAheadAbleToStop(targetCar));
	outputFsm.fsmTransition( 6,  8, new lightAheadGo(targetCar));
	outputFsm.fsmTransition( 7,  8, new lightAheadGo(targetCar));
	outputFsm.fsmTransition( 8,  7, new lightAheadStop(targetCar));
	outputFsm.fsmTransition( 8,  7, new lightAheadAbleToStop(targetCar));
	outputFsm.fsmTransition( 8,  9, new atTarget(targetCar));
	outputFsm.fsmTransition( 9, 10, new arrived(targetCar));
	outputFsm.fsmTransition( 9,  4, new nullTransition());
	outputFsm.fsmTransition(10, 11, new nullTransition());
	outputFsm.fsmTransition(11, 12, new targetClear(targetCar));
	outputFsm.fsmTransition(12, 13, new atTarget(targetCar));
	outputFsm.fsmTransition(13, 14, new lightAheadStop(targetCar));
	outputFsm.fsmTransition(13, 14, new lightAheadAbleToStop(targetCar));
	outputFsm.fsmTransition(13, 15, new lightAheadGo(targetCar));
	outputFsm.fsmTransition(14, 15, new lightAheadGo(targetCar));
	outputFsm.fsmTransition(15, 14, new lightAheadStop(targetCar));
	outputFsm.fsmTransition(15, 14, new lightAheadAbleToStop(targetCar));
	outputFsm.fsmTransition(15, 16, new atTarget(targetCar));
	outputFsm.fsmTransition(16,  3, new arrived(targetCar));
	outputFsm.fsmTransition(16, 11, new nullTransition());

	outputFsm.initialState(1);

	return outputFsm;
}