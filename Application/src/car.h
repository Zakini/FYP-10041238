#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <vector>
#include <string>
#include "../lib/json-master/src/json.hpp"
#include "gameObject.h"
#include "pathEngine.h"
#include "finiteStateMachine.h"
#include "carFsm.h"

using std::vector;
using std::string;

namespace jw
{
	class car : public gameObject
	{
	public:
		friend class carFsm::moveToHome;
		friend class carFsm::pathToHome;
		friend class carFsm::pathToWork;
		friend class carFsm::travelling;
		friend class carFsm::arrived;

		car(pathEngine* p_pather);

		static vector<car*> loadCars(string filepath, pathEngine* pather);
		static vector<car*> loadCars(nlohmann::json carsJson, pathEngine* pather);

		// Inherited via gameObject
		virtual void update(sf::Time timeSinceLastFrame) override;	// TODO UT?
		virtual void draw(sf::RenderWindow& renderTarget) override;	// TODO UT?

	private:
		// TODO separate for UTs?
		sf::Vector2f generateForce();
		void applyForce(sf::Vector2f force, sf::Time period);

		sf::Vector2f position;
		// TODO add currentLocationId - how to update this?
		sf::Vector2f velocity;
		float mass;
		sf::RectangleShape renderShape;
		int homeLocationId, workLocationId;
		pathEngine* pather;
		vector<sf::Vector2f> currentPath;
		fsm controller;
	};
}