#pragma once
#include <vector>
#include "Singleton.h"

namespace HDData
{
	class Tween;
}

namespace HDEngine
{
	class TweenSystem : public Singleton<TweenSystem>
	{
	private:
		friend Singleton;
		TweenSystem() = default;
		~TweenSystem() = default;

	public:
		void Initialize();
		void Update();
		void Finalize();

		HDData::Tween& CreateTween();

		std::vector<HDData::Tween*> _dotweens;
		std::vector<HDData::Tween*> _over;
	};
}


