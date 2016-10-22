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
#include "junctionController.h"
#include <utility>
#include "collidable.h"
#include "collisionDetector.h"

using std::vector;
using std::deque;
using std::string;
using std::shared_ptr;
using std::pair;

namespace jw
{
	class car : public gameObject, public collidable
	{
	public:
		enum situation { none, onRoad, atLight, inJunction };

		car(shared_ptr<pathEngine> p_pather, shared_ptr<collisionDetector> p_carDetector, int p_homeLocationId, int p_workLocationId)
			: car(p_pather, p_carDetector, p_homeLocationId, p_workLocationId, carFsm::generate(*this)) {}
		car(shared_ptr<pathEngine> p_pather, shared_ptr<collisionDetector> p_carDetector, int p_homeLocationId, int p_workLocationId, fsm carController);
		~car();

		static vector<car*> loadCars(string filepath, shared_ptr<pathEngine> pather, shared_ptr<collisionDetector> carDetector);
		static vector<car*> loadCars(nlohmann::json carsJson, shared_ptr<pathEngine> pather, shared_ptr<collisionDetector> carDetector);

		// POSSIBLE most of these were only added for tests, get rid for release?
		int homeLocation() { return homeLocationId; }
		int workLocation() { return workLocationId; }
		int currentLocation() { return currentLocationId; }
		void currentLocation(int locationId);
		deque<int> currentPath() { return _currentPath; }
		void completePathStep();
		shared_ptr<pathEngine> pather() { return _pather; }
		float getMass() { return mass; }
		float getMaxEngineForce() { return maxEngineForce; }
		float getMaxBrakeForce() { return maxBrakeForce; }
		bool isAtPosition(sf::Vector2f target);
		bool isAtTarget();
		pair<const sf::Vector2f*, const junctionController::signalState*> getTrafficLightPerception() { return make_pair(incomingTrafficLightPosition, incomingTrafficLightState); }
		situation getCurrentSituation() { return currentSituation; }

		// Inherited via gameObject
		virtual void update(sf::Time timeSinceLastFrame) override;	// POSSIBLE UT?

		// Inherited via collidable
		virtual sf::Vector2f getVelocity() override { return _velocity; }
		virtual sf::Vector2f getHeading() override;
		virtual sf::Vector2f getPosition() { return _position; }
		virtual sf::FloatRect getBoundingBox() override { return renderShape.getGlobalBounds(); }

		void targetPosition(sf::Vector2f target) { _targetPosition = new sf::Vector2f(target); }
		sf::Vector2f targetPosition() const;
		void pathTo(int targetId);

	private:
		void checkEnvironment();
		void moveTowardTarget(sf::Time period);
		// POSSIBLE separate for UTs?
		sf::Vector2f generateForce(sf::Vector2f target, sf::Time period);
		void applyForce(sf::Vector2f force, sf::Time period);

		// Inherited via gameObject
		virtual void draw(RenderTarget& target, RenderStates states) const override;	// POSSIBLE UT?

		// physics
		sf::Vector2f _position;
		sf::Vector2f _velocity;
		sf::Vector2f _heading;
		float mass;
		const float maxEngineForce;
		const float maxBrakeForce;

		// rendering
		sf::RectangleShape renderShape;

		// pathing
		int previousLocationId, currentLocationId;
		int homeLocationId, workLocationId;
		shared_ptr<pathEngine> _pather;
		deque<int> _currentPath;
		sf::Vector2f* _targetPosition;

		// behaviour
		fsm controller;

		// perceptions
		sf::Vector2f* incomingTrafficLightPosition;
		junctionController::signalState* incomingTrafficLightState;
		shared_ptr<collisionDetector> carDetector;
		sf::Vector2f* safePositionBehindCarAhead;	// TODO rename
		situation currentSituation;

		static const float defaultEngineForce;
		static const float defaultBrakeForce;
		static const float defaultMass;
		static const sf::Vector2f defaultRenderShape;
		static const float arrivalDistanceThreshold;
		static const float arrivalSpeedThreshold;
	};
}