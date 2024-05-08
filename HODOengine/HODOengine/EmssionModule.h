#pragma once
#include "Burst.h"

namespace HDData
{
	class EmissionModule
	{
	public:
		EmissionModule()
		{
			burst = Burst(0.0f, 30);
		}

		Burst GetBurst();
		void SetBurst(Burst burst);

	public:
		bool enabled;
		float rateOverTime;
		float rateOverDistance;

	private:
		Burst burst;
	};
}
