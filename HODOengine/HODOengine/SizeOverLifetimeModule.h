#pragma once
#include "MinMaxCurve.h"

namespace HDData
{
	class SizeOverLifetimeModule
	{
	public:
		SizeOverLifetimeModule()
			: size(1.0f), enabled(false)
		{

		}

		bool enabled;
		MinMaxCurve size;
	};
}
