#pragma once

#include "state.h"
#include "transition.h"
#include "finiteStateMachine.h"

namespace jw
{
	// forward declaration for cyclic dependency
	class car;

	namespace carFsm
	{
		const float arrivalDistanceThreshold = 0.1f;	// POSSIBLE make configurable?
		const float arrivalSpeedThreshold = 1.0f;

		class carState : public state
		{
		public:
			carState(car& p_targetCar) : targetCar(p_targetCar) {}
			virtual ~carState() = default;

			virtual carState* clone() override = 0;

		protected:
			car& targetCar;
		};

		class carTransition : public transition
		{
		public:
			carTransition(car& p_targetCar) : targetCar(p_targetCar) {}
			virtual ~carTransition() = default;

			virtual carTransition* clone() override = 0;

		protected:
			car& targetCar;
		};

		// Teleport car to its home location
		class moveToHome : public carState
		{
		public:
			moveToHome(car& targetCar) : carState(targetCar) {}

			// Inherited via carState
			virtual moveToHome* clone() override { return new moveToHome(*this); }
			virtual void update(sf::Time period) override;
		};

		class pathToHome : public carState
		{
		public:
			pathToHome(car& targetCar) : carState(targetCar) {}

			// Inherited via carState
			virtual pathToHome* clone() override { return new pathToHome(*this); }
			virtual void update(sf::Time period) override;
		};

		class pathToWork : public carState
		{
		public:
			pathToWork(car& targetCar) : carState(targetCar) {}

			// Inherited via carState
			virtual pathToWork* clone() override { return new pathToWork(*this); }
			virtual void update(sf::Time period) override;
		};

		class enterRoad : public carState
		{
		public:
			enterRoad(car& targetCar) : carState(targetCar) {}

			// Inherited via carState
			virtual enterRoad* clone() override { return new enterRoad(*this); }
			virtual void update(sf::Time period) override;
		};

		class followRoad : public carState
		{
		public:
			followRoad(car& targetCar) : carState(targetCar) {}

			// Inherited via carState
			virtual followRoad* clone() override { return new followRoad(*this); }
			virtual void update(sf::Time period) override;
		};

		class updatePath : public carState
		{
		public:
			updatePath(car& targetCar) : carState(targetCar) {}

			// Inherited via carState
			virtual updatePath* clone() override { return new updatePath(*this); }
			virtual void update(sf::Time period) override;
		};

		class arrived : public carTransition
		{
		public:
			arrived(car& targetCar) : carTransition(targetCar) {}

			// Inherited via transition
			virtual arrived* clone() override { return new arrived(*this); }
			virtual bool changeState() override;
		};

		class atRoadStart : public carTransition
		{
		public:
			atRoadStart(car& targetCar) : carTransition(targetCar) {}

			// Inherited via transition
			virtual atRoadStart* clone() override { return new atRoadStart(*this); }
			virtual bool changeState() override;
		};

		class atRoadEnd : public carTransition
		{
		public:
			atRoadEnd(car& targetCar) : carTransition(targetCar) {}

			// Inherited via transition
			virtual atRoadEnd* clone() override { return new atRoadEnd(*this); }
			virtual bool changeState() override;
		};

		fsm generate(car& targetCar);
	}
}