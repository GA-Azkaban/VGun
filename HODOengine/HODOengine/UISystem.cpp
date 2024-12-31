#include "UISystem.h"
#include "Scene.h"
#include "Button.h"
#include "GameObject.h"
#include "UIBase.h"
#include "InputSystem.h"
#include "ObjectSystem.h"
#include "SceneSystem.h"


#include <algorithm>


namespace HDEngine
{
	void UISystem::Initialize()
	{

	}

	void UISystem::Finalize()
	{

	}

	void UISystem::SetChangedScene(HDData::Scene* currentScene)
	{
		for (auto* ui : _uiList)
		{
			ui->SetIsClicked(false);
			ui->SetIsHovering(false);
			ui->SetIsGrabbing(false);
		}

		_uiList.clear();

		for (const auto& obj : ObjectSystem::Instance().GetStaticObjectList())
		{
			for (const auto& comp : obj->GetAllComponents())
			{
				if (HDData::UIBase* one = dynamic_cast<HDData::UIBase*>(comp))
				{
					_uiList.push_back(one);
				}
			}
		}

		for (const auto& obj : ObjectSystem::Instance().GetRunningStaticObjectList())
		{
			for (const auto& comp : obj->GetAllComponents())
			{
				if (HDData::UIBase* one = dynamic_cast<HDData::UIBase*>(comp))
				{
					_uiList.push_back(one);
				}
			}
		}

		for (const auto& obj : currentScene->GetGameObjectList())
		{
			for (const auto& comp : obj->GetAllComponents())
			{
				if (HDData::UIBase* one = dynamic_cast<HDData::UIBase*>(comp))
				{
					_uiList.push_back(one);
				}
			}
		}

		for (const auto& obj : currentScene->GetRunningObjectList())
		{
			for (const auto& comp : obj->GetAllComponents())
			{
				if (HDData::UIBase* one = dynamic_cast<HDData::UIBase*>(comp))
				{
					_uiList.push_back(one);
				}
			}
		}

		// 한 번만 정렬하면 되기 때문에 std::set 보다는 벡터로 만든 뒤 정렬한다.
		std::sort(_uiList.begin(), _uiList.end(), CompareSortOrder);
	}

	bool UISystem::CompareSortOrder(const HDData::UIBase* left, const HDData::UIBase* right)
	{
		return (left->GetSortOrder() > right->GetSortOrder());	// 내림차순
	}

	void UISystem::CheckIsFocused()
	{
		for (const auto& ui : _uiList)
		{
			if (!ui->GetGameObject()->GetParentActive() || !ui->GetGameObject()->GetSelfActive())
			{
				ui->SetIsHovering(false);
				ui->SetIsClicked(false);
				ui->SetIsGrabbing(false);
				continue;
			}

			if (ui->CheckFocus())
			{
				_focusedUI = ui;
				_focusedUI->SetIsHovering(true);

				if (ui->GetIsGrabbing() && InputSystem::Instance().GetMouseUp(MOUSE_LEFT))
				{
					ui->SetIsGrabbing(false);
				}

				if (InputSystem::Instance().GetMouseDown(MOUSE_LEFT))
				{
					_focusedUI->SetIsClicked(true);
					_focusedUI->SetIsGrabbing(true);
					_focusedUI->OnClickEvent();
					break;
				}

				ui->SetIsClicked(false);
				continue;
			}

			ui->SetIsClicked(false);
			ui->SetIsHovering(false);

			if (ui->GetIsGrabbing() && InputSystem::Instance().GetMouseUp(MOUSE_LEFT))
			{
				ui->SetIsGrabbing(false);
			}
		}
	}

	std::vector<HDData::UIBase*>& UISystem::GetEveryUI()
	{
		_everyUI.clear();

		auto& scenes = HDEngine::SceneSystem::Instance().GetAllScenes();

		for (auto& [name, scene] : scenes)
		{
			for (const auto& obj : scene->GetGameObjectList())
			{
				for (const auto& comp : obj->GetAllComponents())
				{
					if (HDData::UIBase* one = dynamic_cast<HDData::UIBase*>(comp))
					{
						_everyUI.push_back(one);
					}
				}
			}

			for (const auto& obj : scene->GetRunningObjectList())
			{
				for (const auto& comp : obj->GetAllComponents())
				{
					if (HDData::UIBase* one = dynamic_cast<HDData::UIBase*>(comp))
					{
						_everyUI.push_back(one);
					}
				}
			}
		}

		return _everyUI;
	}

}
