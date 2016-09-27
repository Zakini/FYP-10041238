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
#include <memory>

using std::vector;
using std::deque;
using std::string;
using std::shared_ptr;

namespace jw
{
	class car : public gameObject
	{
	public:
		car(shared_ptr<pathEngine> p_pather, int p_homeLocationId, int p_workLocationId)
			: car(p_pather, p_homeLocationId, p_workLocationId, carFsm::generate(*this)) {}
		car(shared_ptr<pathEngine> p_pather, int p_homeLocationId, int p_workLocationId, fsm carController);

		static vector<car*> loadCars(string filepath, shared_ptr<pathEngine> pather);
		static vector<car*> loadCars(nlohmann::json carsJson, shared_ptr<pathEngine> pather);

		// most of these were only added for tests, get rid for release?
		int homeLocation() { return homeLocationId; }
		int workLocation() { return workLocationId; }
		sf::Vector2f position() { return _position; }
		sf::Vector2f velocity() { return _velocity; }
		int currentLocation() { return currentLocationID; }
		void currentLocation(int locationId);
		deque<int> currentPath() { return _currentPath; }
		void popStepFromPath();
		shared_ptr<pathEngine> pather() { return _pather; }

		// Inherited via gameObject
		virtual void update(sf::Time timeSinceLastFrame) override;	// POSSIBLE UT?

		void targetPosition(sf::Vector2f target) { _targetPosition = new sf::Vector2f(target); }
		sf::Vector2f targetPosition() const;
		void pathTo(int targetId);

	private:
		void moveTowardTarget(sf::Time period);
		// POSSIBLE separate for UTs?
		sf::Vector2f generateForce(sf::Vector2f target, sf::Time period);
		void applyForce(sf::Vector2f force, sf::Time period);

		// Inherited via gameObject
		virtual void draw(RenderTarget & target, RenderStates states) const override;	// POSSIBLE UT?

		sf::Vector2f _position;
		int currentLocationID;
		sf::Vector2f _velocity;
		float mass;
		const float maxEngineForce;
		const float maxBrakeForce;
		sf::RectangleShape renderShape;
		int homeLocationId, workLocationId;
		shared_ptr<pathEngine> _pather;
		deque<int> _currentPath;
		sf::Vector2f* _targetPosition;
		fsm controller;

		static const float defaultEngineForce;
		static const float defaultBrakeForce;
		static const float defaultMass;
		static const sf::Vector2f defaultRenderShape;

	};
}