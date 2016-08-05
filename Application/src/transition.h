#pragma once

namespace jw
{
	class transition
	{
	public:
		virtual bool changeState() = 0;
	};
}