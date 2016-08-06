#include "car.h"

#include <fstream>

using std::ifstream;

jw::car::car(pathEngine* p_pather)
	: position(0, 0)
	, velocity(0, 0)
	, mass(1)
	, renderShape(sf::Vector2f(2, 2))
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
	return sf::Vector2f();	// TODO
}

void jw::car::applyForce(sf::Vector2f force, sf::Time period)
{
	sf::Vector2f acceleration = force / mass;
	sf::Vector2f newVelocity = velocity + acceleration * period.asSeconds();
	position += velocity * period.asSeconds();
}