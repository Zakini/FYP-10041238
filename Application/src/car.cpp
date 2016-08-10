#include "car.h"

#include <fstream>
#include <math.h>

using std::ifstream;

const float jw::car::defaultEngineForce = 100000.0f;
const float jw::car::defaultBrakeForce = 200000.0f;
const float jw::car::defaultMass = 1.0f;
const sf::Vector2f jw::car::defaultRenderShape = { 4, 4 };

jw::car::car(pathEngine* p_pather)
	: position(0, 0)
	, currentLocationID(0)
	, velocity(0, 0)
	, mass(defaultMass)
	, maxEngineForce(defaultEngineForce)
	, maxBrakeForce(defaultBrakeForce)
	, renderShape(defaultRenderShape)
	, homeLocationId(0)
	, workLocationId(0)
	, pather(p_pather)
	, controller(carFsm::generate(*this))
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
		car* newCar = new car(pather);

		newCar->homeLocationId = carJson[homeKey];
		newCar->workLocationId = carJson[workKey];

		outputVector.push_back(newCar);
	}

	return outputVector;
}

void jw::car::update(sf::Time timeSinceLastFrame)
{
	controller.update(timeSinceLastFrame);

	renderShape.setPosition(position);
}

void jw::car::draw(sf::RenderWindow& renderTarget)
{
	renderTarget.draw(renderShape);
}

sf::Vector2f jw::car::generateForce()
{
	// Move to target, aim to be stationary on arrival
	sf::Vector2f outputForce;

	float speed = length(velocity);
	float currentStoppingDistance = std::pow(speed, 2) / (2 * maxBrakeForce);

	int targetLocationId = currentPath.front();
	sf::Vector2f targetPosition = pather->getLocationPosition(targetLocationId);
	sf::Vector2f vectorToTarget = targetPosition - position;
	float distanceFromTarget = length(vectorToTarget);
	sf::Vector2f targetDirection;
	if (distanceFromTarget != 0)
	{
		targetDirection = vectorToTarget / distanceFromTarget;
	}
	// else targetDirection is {0,0}

	if (distanceFromTarget > currentStoppingDistance)
	{
		// accelerate
		outputForce += targetDirection * maxEngineForce;
	}
	else
	{
		// decelerate
		// negate direction for opposite braking force
		outputForce += -velocity * maxBrakeForce;
	}

	// friction
	const float frictionCoefficient = 0.3f;
	sf::Vector2f friction = -velocity * (frictionCoefficient * mass * 10);	// mass * 10 = normal force

	return outputForce;
}

void jw::car::applyForce(sf::Vector2f force, sf::Time period)
{
	sf::Vector2f acceleration = force / mass;
	velocity += acceleration * period.asSeconds();
	position += velocity * period.asSeconds();
}