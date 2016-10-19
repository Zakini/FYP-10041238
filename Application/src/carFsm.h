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

		class targetRoadStart : public carState
		{
		public:
			targetRoadStart(car& targetCar) : carState(targetCar) {}

			// Inherited via carState
			virtual targetRoadStart* clone() override { return new targetRoadStart(*this); }
			virtual void update(sf::Time period) override;
		};

		class targetRoadEnd : public carState
		{
		public:
			targetRoadEnd(car& targetCar) : carState(targetCar) {}

			// Inherited via carState
			virtual targetRoadEnd* clone() override { return new targetRoadEnd(*this); }
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

		class atTarget : public carTransition
		{
		public:
			atTarget(car& targetCar) : carTransition(targetCar) {}

			// Inherited via transition
			virtual atTarget* clone() override { return new atTarget(*this); }
			virtual bool changeState() override;
		};

		class lightAheadStop : public carTransition
		{
		public:
			lightAheadStop(car& targetCar) : carTransition(targetCar) {}

			// Inherited via transition
			virtual lightAheadStop* clone() override { return new lightAheadStop(*this); }
			virtual bool changeState() override;
		};

		class lightAheadAbleToStop : public carTransition
		{
		public:
			lightAheadAbleToStop(car& targetCar) : carTransition(targetCar) {}

			// Inherited via transition
			virtual lightAheadAbleToStop* clone() override { return new lightAheadAbleToStop(*this); }
			virtual bool changeState() override;
		};

		class lightAheadGo : public carTransition
		{
		public:
			lightAheadGo(car& targetCar) : carTransition(targetCar) {}

			// Inherited via transition
			virtual lightAheadGo* clone() override { return new lightAheadGo(*this); }
			virtual bool changeState() override;
		};

		fsm generate(car& targetCar);
	}
}