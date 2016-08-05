#include "car.h"

#include <fstream>

using std::ifstream;

jw::car::car(sf::Vector2f p_position, pathEngine* p_pather)
	: position(p_position), velocity(), mass(1), renderShape(sf::Vector2f(2, 2)), pather(p_pather), controller(carFsm::generate(*this))
{
	renderShape.setOrigin(renderShape.getSize() / 2.0f);
	renderShape.setFillColor(sf::Color::Green);

	controller.initialise();
}

vector<jw::car*> jw::car::loadCars(string filepath)
{
	ifstream carFile(filepath);
	nlohmann::json carJson;

	carFile >> carJson;

	return loadCars(carJson);
}

vector<jw::car*> jw::car::loadCars(nlohmann::json carsJson)
{
	const string carsKey = "cars";
	const string positionKey = "position";
	const string xKey = "x";
	const string zKey = "z";

	vector<car*> outputVector;

	for (auto& carJson : carsJson[carsKey])
	{
		nlohmann::json positionJson = carJson.at(positionKey);
		sf::Vector2f position;
		position.x = positionJson.at(xKey);
		position.y = positionJson.at(zKey);

		car* newCar = new car(position);
		outputVector.push_back(newCar);
	}

	return outputVector;
}

void jw::car::update(sf::Time timeSinceLastFrame)
{
	controller.update();

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