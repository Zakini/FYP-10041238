#include "carFsm.h"
#include "car.h"

#include <SFML/System/Vector2.hpp>

void jw::carFsm::moveToHome::update(sf::Time period)
{
	sf::Vector2f homePosition = targetCar.pather->getLocationPosition(targetCar.homeLocationId);
	targetCar.position = homePosition;
}

void jw::carFsm::findPath::update(sf::Time period)
{
	// TODO
}

void jw::carFsm::travelling::update(sf::Time period)
{
	sf::Vector2f newForce = targetCar.generateForce();
	targetCar.applyForce(newForce, period);
}

bool jw::carFsm::arrived::changeState()
{
	return false;	// TODO is car at target location?
}

jw::fsm jw::carFsm::generate(car& targetCar)
{
	fsm outputFsm;

	outputFsm.addState(1, new moveToHome(targetCar));
	outputFsm.addState(2, new findPath(targetCar));
	outputFsm.addState(3, new travelling(targetCar));

	outputFsm.addTransition(1, 2, new nullTransition());
	outputFsm.addTransition(2, 3, new nullTransition());
	outputFsm.addTransition(3, 2, new arrived(targetCar));

	outputFsm.setInitialState(1);

	return outputFsm;
}