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
		friend class carFsm::travelling;
		friend class carFsm::arrived;

		car(sf::Vector2f p_position = {0, 0}, pathEngine* p_pather = nullptr);

		static vector<car*> loadCars(string filepath);
		static vector<car*> loadCars(nlohmann::json carsJson);

		// Inherited via gameObject
		virtual void update(sf::Time timeSinceLastFrame) override;	// TODO UT?
		virtual void draw(sf::RenderWindow& renderTarget) override;	// TODO UT?

	private:
		// TODO separate for UTs?
		sf::Vector2f generateForce();
		void applyForce(sf::Vector2f force, sf::Time period);

		sf::Vector2f position;
		sf::Vector2f velocity;
		float mass;
		sf::RectangleShape renderShape;
		pathEngine* pather;
		fsm controller;
	};
}