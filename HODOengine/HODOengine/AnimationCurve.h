#pragma once
#include "dllExporter.h"
#include <map>
#include <functional>

namespace HDData
{
	class HODO_API AnimationCurve
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
			
			auto currentKey = keys.end();
			for (auto iter = keys.begin(); iter != keys.end(); ++iter)
			{
				if (iter->first.first <= time && iter->first.second >= time)
				{
					currentKey = iter;
					break;
				}
			}

			if (currentKey == keys.end())
			{
				if (keys.begin()->first.first >= time)
				{
					return keys.begin()->second(keys.begin()->first.first);
				}
				else
				{
					return keys.rbegin()->second(keys.rbegin()->first.second);
				}
			}
			else
			{
				return currentKey->second(time);
			}
		}

	private:
		// <<startTime, endTime>, function>
		std::map<std::pair<float, float>, std::function<float(float)>> keys;
	};
}
