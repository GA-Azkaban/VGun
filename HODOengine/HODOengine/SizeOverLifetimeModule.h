#pragma once
#include <map>
#include <functional>

namespace HDData
{
	class SizeOverLifetimeModule
	{
	public:
		bool enabled = false;
		std::map<float, std::function<float(float)>> size;
	};
}
