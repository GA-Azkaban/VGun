#pragma once
#include "MinMaxCurve.h"

namespace HDData
{
	class RotationOverLifetimeModule
	{
	public:
		RotationOverLifetimeModule()
			: angularVelocity(45.0f), enabled(false)
		{

		}
		bool enabled;
		MinMaxCurve angularVelocity;
	};
}
