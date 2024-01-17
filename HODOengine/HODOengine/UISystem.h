#pragma once
#include <set>

#include "Singleton.h"
#include "SceneSystem.h"

namespace HDData
{
	class Scene;
	class UIBase;
}

namespace HDEngine
{
	class UISystem : public Singleton<UISystem>
	{
		friend UISystem;

	private:
		UISystem() = default;

	public:
		void Initialize();
		void Update();
		void Finalize();

		bool CheckIsFocused();
		bool CheckIsClicked();

	private:
		HDData::Scene* _currentScene;
		std::set<HDData::UIBase*> _uiList;
	};
}


