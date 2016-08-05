#pragma once

namespace jw
{
	class transition
	{
	public:
		virtual bool changeState() = 0;
	};

	class nullTransition : public transition
	{
		// Inherited via transition
		virtual bool changeState() override { return true; };
	};
}