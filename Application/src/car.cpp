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
const float jw::car::defaultMass = 1.0f;
const sf::Vector2f jw::car::defaultRenderShape = { 4, 4 };
const float defaultRenderDepth = 10.0f;

const float gravitationalConstant = 9.81f;

jw::car::car(pathEngine* p_pather, int p_homeLocationId, int p_workLocationId)
	: _position(0, 0)
	, currentLocationID(0)
	, velocity(0, 0)
	, mass(defaultMass)
	, maxEngineForce(defaultEngineForce)
	, maxBrakeForce(defaultBrakeForce)
	, renderShape(defaultRenderShape)
	, homeLocationId(p_homeLocationId)
	, workLocationId(p_workLocationId)
	, pather(p_pather)
	, controller(carFsm::generate(*this))
	, gameObject(defaultRenderDepth)
{
	renderShape.setOrigin(renderShape.getSize() / 2.0f);
	renderShape.setFillColor(sf::Color::Green);

	controller.initialise();
}

vector<jw::car*> jw::car::loadCars(string filepath, pathEngine* pather)
{
	ifstream carFile(filepath);
	nlohmann::json carJson;

	carFile >> carJson;

	return loadCars(carJson, pather);
}

vector<jw::car*> jw::car::loadCars(nlohmann::json carsJson, pathEngine* pather)
{
	const string carsKey = "cars";
	const string homeKey = "home";
	const string workKey = "work";

	vector<car*> outputVector;

	for (auto& carJson : carsJson[carsKey])
	{
		car* newCar = new car(pather, carJson[homeKey], carJson[workKey]);
		outputVector.push_back(newCar);
	}

	return outputVector;
}

// Note: may need to set/reset other values as they're added
void jw::car::currentLocation(int locationId)
{
	// check location exists (throws if invalid)
	sf::Vector2f newPosition = pather->getLocationPosition(locationId);

	// set new values
	currentLocationID = locationId;
	_position = newPosition;

	// reset values that might be invalid now
	velocity.x = 0;
	velocity.y = 0;
	_currentPath.clear();
}

void jw::car::update(sf::Time timeSinceLastFrame)
{
	controller.update(timeSinceLastFrame);

	renderShape.setPosition(_position);
}

void jw::car::draw(sf::RenderWindow& renderTarget)
{
	renderTarget.draw(renderShape);
}

void jw::car::pathTo(int targetId)
{
	deque<int> newPath = pather->findPath(currentLocationID, targetId);

	if (!_currentPath.empty())
	{
		// continue to the next node before starting new path
		newPath.push_front(_currentPath.front());
	}

	_currentPath = newPath;
}

// Move to target, aim to be stationary on arrival
void jw::car::generateForce(sf::Time period)
{
	// Calculate force
	sf::Vector2f newForce;

	float speed = length(velocity);
	float currentStoppingDistance = std::pow(speed, 2) / (2 * maxBrakeForce);

	int targetLocationId = _currentPath.front();
	sf::Vector2f targetPosition = pather->getRoadEndPosition(currentLocationID, targetLocationId);
	sf::Vector2f vectorToTarget = targetPosition - _position;
	float distanceFromTarget = length(vectorToTarget);
	
	if (distanceFromTarget <= currentStoppingDistance || distanceFromTarget <= carFsm::arrivalThreshold)
	{
		if (speed > 0)
		{
			// decelerate
			// negate direction for opposite braking force
			sf::Vector2f direction = velocity / speed;
			newForce = -direction * maxBrakeForce;
		}// else already stopped
	}
	else	// far from target
	{
		// accelerate
		sf::Vector2f trajectoryForCurrentStep = velocity * period.asSeconds();
		sf::Vector2f idealForce = (vectorToTarget - trajectoryForCurrentStep) / period.asSeconds();	// HACK something to do with removing the time component of the "force"... iunno

		if (length(idealForce) <= maxEngineForce)
		{
			// accelerate gently if close to target
			newForce = idealForce;
		}
		else
		{
			sf::Vector2f forceDirection = normalise(idealForce);
			newForce = forceDirection * maxEngineForce;
		}
	}

	// friction
	const float frictionCoefficient = 0.3f;
	// negate velocity for opposite force
	sf::Vector2f friction = -velocity * (frictionCoefficient * mass * gravitationalConstant);	// mass * gravity = normal force
	newForce += friction;

	// Apply force
	// TODO separate again
	sf::Vector2f acceleration = newForce / mass;
	sf::Vector2f newVelocity = velocity + (acceleration * period.asSeconds());

	if (angleBetween(velocity, newVelocity) > 90)	// Car is completely stopping/turning around
	{
		// full stop
		velocity.x = 0;
		velocity.y = 0;
	}
	else
	{
		velocity = newVelocity;
	}

	_position += velocity * period.asSeconds();
}