#include "UIBase.h"
#include "Transform.h"
#include "InputSystem.h"

namespace HDData
{
	UIBase::UIBase()
		:_sketchable(NULL)
	{

	}

	void UIBase::UpdateRenderData()
	{
		for (const auto& one : _sketchable)
		{
			one->SetWorldTM(GetTransform()->GetWorldTM());
		}

	}

	bool UIBase::CheckFocus()
	{
		if (_ignoreFocus)
		{
			return false;
		}

		HDMath::HDFLOAT2 mouse = HDEngine::InputSystem::Instance().GetMousePosition();

		return true;
	}

	int UIBase::GetSortOrder() const
	{
		return _sortOrder;
	}

	bool UIBase::GetIsFocused()
	{
		return _isFocused;
	}

	bool UIBase::GetIsClicked()
	{
		return _isClicked;
	}

	bool UIBase::IsIgnoreFocused()
	{
		return _ignoreFocus;
	}

	float UIBase::GetLeft()
	{
		return {};
	}

	float UIBase::GetRight()
	{
		return {};
	}

	float UIBase::GetTop()
	{
		return {};
	}

	float UIBase::GetBottom()
	{
		return {};
	}

}
