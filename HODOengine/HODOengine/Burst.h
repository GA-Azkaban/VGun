#pragma once
namespace HDData
{
	class Burst
	{
	public:
		Burst(float _time, int _count)
			: time(_time), count(_count), minCount(-1), maxCount(-1), cycleCount(1), repeatInterval(0.01f)
		{

		}
		Burst(float _time, int _minCount, int _maxCount)
			: time(_time), count(-1), minCount(_minCount), maxCount(_maxCount), cycleCount(1), repeatInterval(0.01f)
		{

		}
		Burst(float _time, int _minCount, int _maxCount, int _cycleCount, float _repeatInterval)
			: time(_time), count(-1), minCount(_minCount), maxCount(_maxCount), 
			cycleCount(_cycleCount), repeatInterval(_repeatInterval)
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
	};
}
