#pragma once

namespace jw
{
	class transition
	{
	public:
		virtual ~transition() = default;
		virtual transition* clone() = 0;

		virtual bool changeState() = 0;
	};

	class nullTransition : public transition
	{
		// Inherited via transition
		virtual nullTransition* clone() override { return new nullTransition(*this); }
		virtual bool changeState() override { return true; };
	};
}