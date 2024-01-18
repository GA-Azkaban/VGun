#pragma once
#include <vector>
#include <set>

#include "Singleton.h"

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

	public:
		UISystem() = default;

		void Initialize();
		void Update();
		void Finalize();

		void SetChangedScene(HDData::Scene* currentScene);
		static bool CompareSortOrder(const HDData::UIBase* left, const HDData::UIBase* right);

		void CheckIsFocused();

	private:
		std::vector<HDData::UIBase*> _uiList;
		HDData::UIBase* _focusedUI;
	};
}


