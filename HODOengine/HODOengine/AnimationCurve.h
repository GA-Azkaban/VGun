#pragma once
#include <map>
#include <functional>

namespace HDData
{
	class AnimationCurve
	{
	public:
		void AddKey(float startTime, float endTime, std::function<float(float)> function)
		{
			keys.insert(std::make_pair(std::make_pair(startTime, endTime), function));
		}

		float Evaluate(float time)
		{
			if (keys.empty())
			{
				return time;
			}
			
			for (auto iter = keys.begin(); iter != keys.end(); ++iter)
			{
				if (iter->first.second <= time)
				{
					return iter->second(time);
				}
			}

			// keys에 등록되지 않은 시간이 매개변수로 들어왔을 때의 처리는 나중에 추가한다.
		}

	private:
		// <<startTime, endTime>, function>
		std::map<std::pair<float, float>, std::function<float(float)>> keys;
	};
}
