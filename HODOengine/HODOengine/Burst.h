#pragma once
namespace HDData
{
	class Burst
	{
	public:
		friend class ParticleSystem;

		Burst(float _time, int _count)
			: time(_time), count(_count), minCount(-1), maxCount(-1), cycleCount(1), repeatInterval(0.01f),
			currentCycleCount(0)
		{

		}
		Burst(float _time, int _minCount, int _maxCount)
			: time(_time), count(-1), minCount(_minCount), maxCount(_maxCount), cycleCount(1), repeatInterval(0.01f),
			currentCycleCount(0)
		{

		}
		Burst(float _time, int _minCount, int _maxCount, int _cycleCount, float _repeatInterval)
			: time(_time), count(-1), minCount(_minCount), maxCount(_maxCount), 
			cycleCount(_cycleCount), repeatInterval(_repeatInterval),
			currentCycleCount(0)
		{

		}

	public:
		float time;
		int count;
		int minCount;
		int maxCount;
		int cycleCount;
		float repeatInterval;
		//float probability;

	private:
		int currentCycleCount;
	};
}
