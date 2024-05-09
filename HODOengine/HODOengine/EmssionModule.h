#pragma once
#include "Burst.h"

namespace HDData
{
	class EmissionModule
	{
	public:
		EmissionModule()
			: burst(0.0f, 30), enabled(true), rateOverTime(10), rateOverDistance(0)
		{
			
		}

		Burst GetBurst() const
		{
			return burst;
		}
		void SetBurst(const Burst& burst)
		{
			this->burst = burst;
		}

	public:
		bool enabled;
		int rateOverTime;
		int rateOverDistance;

	private:
		Burst burst;
	};
}
