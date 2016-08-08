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
		const float arrivalThreshold = 1.0f;	// TODO err... actually use this?

		class carState : public state
		{
		public:
			carState(car& p_targetCar) : targetCar(p_targetCar) {}

		protected:
			car& targetCar;
		};

		class carTransition : public transition
		{
		public:
			carTransition(car& p_targetCar) : targetCar(p_targetCar) {}

		protected:
			car& targetCar;
		};

		class moveToHome : public carState
		{
		public:
			moveToHome(car& targetCar) : carState(targetCar) {}

			// Inherited via carState
			virtual void update(sf::Time period) override;
		};

		class pathToHome : public carState
		{
		public:
			pathToHome(car& targetCar) : carState(targetCar) {}

			// Inherited via carState
			virtual void update(sf::Time period) override;
		};

		class pathToWork : public carState
		{
		public:
			pathToWork(car& targetCar) : carState(targetCar) {}

			// Inherited via carState
			virtual void update(sf::Time period) override;
		};

		class travelling : public carState
		{
		public:
			travelling(car& targetCar) : carState(targetCar) {}

			// Inherited via carState
			virtual void update(sf::Time period) override;
		};

		class arrived : public carTransition
		{
		public:
			arrived(car& targetCar) : carTransition(targetCar) {}

			// Inherited via transition
			virtual bool changeState() override;
		};

		fsm generate(car& targetCar);
	}
}