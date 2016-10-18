#include "car.h"

#include <fstream>
#include <math.h>
#include <stdexcept>
#include "vectorMaths.h"

using std::ifstream;
using std::out_of_range;
using jw::maths::length;
using jw::maths::normalise;
using jw::maths::angleBetween;

const float jw::car::defaultEngineForce = 100000.0f;
const float jw::car::defaultBrakeForce = 200000.0f;
const float jw::car::defaultMass = 1500.0f;
const sf::Vector2f jw::car::defaultRenderShape = { 4, 4 };
const float defaultRenderDepth = 10.0f;
const float jw::car::arrivalDistanceThreshold = 0.1f;	// POSSIBLE make configurable?
const float jw::car::arrivalSpeedThreshold = 1.0f;

const float gravitationalConstant = 9.81f;

jw::car::car(shared_ptr<pathEngine> p_pather, shared_ptr<collisionDetector> p_carDetector, int p_homeLocationId, int p_workLocationId, fsm carController)
	: _position(0, 0)
	, _velocity(0, 0)
	, mass(defaultMass)
	, maxEngineForce(defaultEngineForce)
	, maxBrakeForce(defaultBrakeForce)
	, renderShape(defaultRenderShape)
	, previousLocationId(0)
	, currentLocationId(0)
	, homeLocationId(p_homeLocationId)
	, workLocationId(p_workLocationId)
	, _pather(p_pather)
	, _currentPath()
	, _targetPosition(nullptr)
	, controller(carController)
	, incomingTrafficLightPosition(nullptr)
	, incomingTrafficLightState(nullptr)
	, carDetector(p_carDetector)
	, safePositionBehindCarAhead(nullptr)
	, gameObject(defaultRenderDepth)
{
	renderShape.setOrigin(renderShape.getSize() / 2.0f);
	renderShape.setFillColor(sf::Color::Green);

	controller.initialise();
}

jw::car::~car()
{
	delete _targetPosition;
	delete incomingTrafficLightPosition;
	delete incomingTrafficLightState;
}

vector<jw::car*> jw::car::loadCars(string filepath, shared_ptr<pathEngine> pather, shared_ptr<collisionDetector> carDetector)
{
	ifstream carFile(filepath);
	nlohmann::json carJson;

	carFile >> carJson;

	return loadCars(carJson, pather, carDetector);
}

vector<jw::car*> jw::car::loadCars(nlohmann::json carsJson, shared_ptr<pathEngine> pather, shared_ptr<collisionDetector> carDetector)
{
	const string carsKey = "cars";
	const string homeKey = "home";
	const string workKey = "work";

	vector<car*> outputVector;

	for (auto& carJson : carsJson[carsKey])
	{
		car* newCar = new car(pather, carDetector, carJson[homeKey], carJson[workKey]);
		outputVector.push_back(newCar);
	}

	return outputVector;
}

// Note: may need to set/reset other values as they're added
void jw::car::currentLocation(int locationId)
{
	// check location exists (throws if invalid)
	sf::Vector2f newPosition = _pather->getLocationPosition(locationId);

	// set new values
	currentLocationId = locationId;
	previousLocationId = locationId;
	_position = newPosition;

	// reset values that might be invalid now
	_velocity.x = 0;
	_velocity.y = 0;
	_currentPath.clear();
}

void jw::car::popStepFromPath()
{
	previousLocationId = currentLocationId;
	currentLocationId = _currentPath.front();
	_currentPath.pop_front();
}

bool jw::car::isAtPosition(sf::Vector2f target)
{
	float distanceFromTarget = length(_position - target);
	float currentSpeed = length(_velocity);

	if (distanceFromTarget <= arrivalDistanceThreshold && currentSpeed < arrivalSpeedThreshold) return true;
	else return false;
}

bool jw::car::isAtTarget()
{
	if (_targetPosition == nullptr) return false;
	else return isAtPosition(*_targetPosition);
}

void jw::car::update(sf::Time timeSinceLastFrame)
{
	controller.update(timeSinceLastFrame);

	checkEnvironment();

	moveTowardTarget(timeSinceLastFrame);

	renderShape.setPosition(_position);
}

void jw::car::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(renderShape);
}

sf::Vector2f jw::car::targetPosition() const
{
	if (_targetPosition == nullptr) throw out_of_range("target position not set");

	return *_targetPosition;
}

void jw::car::pathTo(int targetId)
{
	deque<int> newPath = _pather->findPath(currentLocationId, targetId);

	if (!_currentPath.empty())
	{
		// continue to the next node before starting new path
		newPath.push_front(_currentPath.front());
	}

	_currentPath = newPath;
}

void jw::car::checkEnvironment()
{
	// ##################
	// ### check cars ###
	// ##################

	safePositionBehindCarAhead = nullptr;

	if (carDetector != nullptr)
	{
		sf::Time minTimeToFullStop = sf::seconds((-length(_velocity)) * (-(mass / maxBrakeForce)));

		// TODO check they are actually ahead! the front car doesn't need to respond and it does currently!
		// find cars ahead
		auto carsAhead = carDetector->predictCollisions(*this, minTimeToFullStop, renderShape.getGlobalBounds().width,
			[this](collidable* collidee)
		{
			return angleBetween(this->getHeading(), collidee->getPosition() - this->getPosition()) < 90;
		});

		if (carsAhead.size() > 0)
		{
			// store safe position behind closest car ahead temporarily
			auto tempCarPosition = std::min_element(carsAhead.begin(), carsAhead.end(),
				[](sf::Vector2f a, sf::Vector2f b) { return length(a) < length(b); });

			safePositionBehindCarAhead = new Vector2f(*tempCarPosition);
		}

		// TODO find cars behind
	}

	// ############################
	// ### check traffic lights ###
	// ############################

	// no incoming light or light is now behind us
	if (incomingTrafficLightPosition == nullptr || angleBetween(_velocity, *incomingTrafficLightPosition - _position) > 90.0f)
	{
		// find next light
		if (_currentPath.size() > 0 && _pather->isLocationTrafficControlled(_currentPath.front()))
		{
			incomingTrafficLightPosition = new Vector2f(_pather->getRoadEndPosition(currentLocationId, _currentPath.front()));
		}
		else
		{
			incomingTrafficLightPosition = nullptr;
		}
	}

	incomingTrafficLightState = nullptr;

	// we have a light incoming/found a new one
	if (incomingTrafficLightPosition != nullptr)
	{
		if (isAtPosition(*incomingTrafficLightPosition))	// already waiting at light
		{
			// check light at end of previous road
			incomingTrafficLightState = new junctionController::signalState(_pather->getSignalAtRoadEnd(previousLocationId, currentLocationId));
		}
		else if (_targetPosition != nullptr && *_targetPosition == *incomingTrafficLightPosition)	// heading towards this light
		{
			// check light at end of next road
			incomingTrafficLightState = new junctionController::signalState(_pather->getSignalAtRoadEnd(currentLocationId, _currentPath.front()));
		}
	}
}

void jw::car::moveTowardTarget(sf::Time period)
{
	if (!_targetPosition) return;	// no target, no need to move

	sf::Vector2f tempTarget = *_targetPosition;

	// car ahead, need to slow down
	if (safePositionBehindCarAhead != nullptr)
	{
		tempTarget = *safePositionBehindCarAhead;
	}
	// light ahead is red
	else if (incomingTrafficLightState != nullptr && *incomingTrafficLightState == junctionController::signalState::stop)
	{
		tempTarget = *incomingTrafficLightPosition;
	}
	else if (false)	// TODO if traffic light is stopIfAble, only stop if there are no cars close behind
	{

	}
	else	// TODO if traffic light is go or prepareToGo or there is no traffic light, no need to stop completely, go through junction at certain speed
	{

	}

	sf::Vector2f force = generateForce(tempTarget, period);
	applyForce(force, period);
}

// TODO arc paths, rather than straight, when turning
// Move to target, aim to be stationary on arrival
sf::Vector2f jw::car::generateForce(sf::Vector2f target, sf::Time period)
{
	// Calculate force
	sf::Vector2f force;

	float speed = length(_velocity);
	float currentStoppingDistance = std::pow(speed, 2) / (2 * maxBrakeForce);

	sf::Vector2f vectorToTarget = target - _position;
	float distanceFromTarget = length(vectorToTarget);
	
	if (distanceFromTarget <= currentStoppingDistance || distanceFromTarget <= arrivalDistanceThreshold)
	{
		if (speed > 0)
		{
			// decelerate
			// negate direction for opposite braking force
			sf::Vector2f direction = _velocity / speed;
			force = -direction * maxBrakeForce;
		}// else already stopped
	}
	else	// far from target
	{
		// accelerate
		sf::Vector2f idealVelocity = vectorToTarget / period.asSeconds();
		sf::Vector2f idealAcceleration = (idealVelocity - _velocity) / period.asSeconds();
		sf::Vector2f idealForce = mass * idealAcceleration;

		if (length(idealForce) <= maxEngineForce)
		{
			// accelerate gently if close to target
			force = idealForce;
		}
		else
		{
			sf::Vector2f forceDirection = normalise(idealForce);
			force = forceDirection * maxEngineForce;
		}
	}

	// friction
	const float frictionCoefficient = 0.3f;
	// negate velocity for opposite force
	sf::Vector2f friction = -_velocity * (frictionCoefficient * mass * gravitationalConstant);	// mass * gravity = normal force
	force += friction;

	return force;
}

void jw::car::applyForce(sf::Vector2f force, sf::Time period)
{
	sf::Vector2f acceleration = force / mass;
	sf::Vector2f newVelocity = _velocity + (acceleration * period.asSeconds());

	if (angleBetween(_velocity, newVelocity) > 90)	// Car is completely stopping/turning around
	{
		// full stop
		_velocity.x = 0;
		_velocity.y = 0;
	}
	else
	{
		_velocity = newVelocity;
	}

	if (length(_velocity) != 0)
	{
		_heading = normalise(_velocity);
	}// else leave heading as it was

	_position += _velocity * period.asSeconds();
}