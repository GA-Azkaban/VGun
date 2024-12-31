#pragma once
#include <vector>
#include <set>

#include "Singleton.h"

namespace HDData
{
	class Scene;
	class Button;
	class UIBase;
}

namespace HDEngine
{
	class UISystem : public Singleton<UISystem>
	{
		friend Singleton;

	public:
		UISystem() = default;

		void Initialize();
		void Finalize();

		void SetChangedScene(HDData::Scene* currentScene);
		static bool CompareSortOrder(const HDData::UIBase* left, const HDData::UIBase* right);

		void CheckIsFocused();

	private:
		std::vector<HDData::UIBase*> _uiList;
		HDData::UIBase* _focusedUI;

	public:
		std::vector<HDData::UIBase*>& GetEveryUI();

	private:
		std::vector<HDData::UIBase*> _everyUI;

	};
}
