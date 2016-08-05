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

		class findPath : public carState
		{
		public:
			findPath(car& targetCar) : carState(targetCar) {}

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