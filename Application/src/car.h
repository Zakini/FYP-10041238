#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <vector>
#include <deque>
#include <string>
#include "../lib/json-master/src/json.hpp"
#include "gameObject.h"
#include "pathEngine.h"
#include "finiteStateMachine.h"
#include "carFsm.h"
#include "vectorMaths.h"

using std::vector;
using std::deque;
using std::string;

namespace jw
{
	class car : public gameObject
	{
	public:
		friend class carFsm::moveToHome;
		friend class carFsm::pathToHome;
		friend class carFsm::pathToWork;
		friend class carFsm::enterRoad;
		friend class carFsm::followRoad;
		friend class carFsm::updatePath;
		friend class carFsm::arrived;
		friend class carFsm::atRoadStart;
		friend class carFsm::atRoadEnd;

		car(pathEngine* p_pather, int p_homeLocationId, int p_workLocationId);

		static vector<car*> loadCars(string filepath, pathEngine* pather);
		static vector<car*> loadCars(nlohmann::json carsJson, pathEngine* pather);

		// most of these were only added for tests, get rid for release?
		int homeLocation() { return homeLocationId; }
		int workLocation() { return workLocationId; }
		sf::Vector2f position() { return _position; }
		int currentLocation() { return currentLocationID; }
		void currentLocation(int locationId);
		deque<int> currentPath() { return _currentPath; }

		// Inherited via gameObject
		virtual void update(sf::Time timeSinceLastFrame) override;	// POSSIBLE UT?
		virtual void draw(sf::RenderWindow& renderTarget) override;	// POSSIBLE UT?

		void pathTo(int targetId);

	private:
		// POSSIBLE separate for UTs?
		sf::Vector2f generateForce(sf::Vector2f target, sf::Time period);
		void applyForce(sf::Vector2f force, sf::Time period);

		sf::Vector2f _position;
		int currentLocationID;
		sf::Vector2f velocity;
		float mass;
		const float maxEngineForce;
		const float maxBrakeForce;
		sf::RectangleShape renderShape;
		int homeLocationId, workLocationId;
		pathEngine* pather;
		deque<int> _currentPath;
		fsm controller;

		static const float defaultEngineForce;
		static const float defaultBrakeForce;
		static const float defaultMass;
		static const sf::Vector2f defaultRenderShape;
	};
}