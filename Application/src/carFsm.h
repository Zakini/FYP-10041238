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
		const float arrivalThreshold = 1.0f;

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

		class travelling : public carState
		{
		public:
			travelling(car& targetCar) : carState(targetCar) {}

			// Inherited via carState
			virtual travelling* clone() override { return new travelling(*this); }
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

		fsm generate(car& targetCar);
	}
}