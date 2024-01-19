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
		_sketchable->SetWorldTM(GetTransform()->GetWorldTM());
	}

	bool UIBase::CheckFocus()
	{
		if (_ignoreFocus)
		{
			return false;
		}

		Vector2 mouse = HDEngine::InputSystem::Instance().GetMousePosition();
		
		auto mouseX = mouse.x;
		auto mouseY = mouse.y;
		auto left = GetLeft();
		auto right = GetRight();
		auto top = GetTop();
		auto bottom = GetBottom();

		if (mouse.x > GetLeft() &&
			mouse.y > GetTop() &&
			mouse.x < GetRight() &&
			mouse.y < GetBottom())
		{
			return true;
		}

		return false;
	}

	int UIBase::GetSortOrder() const
	{
		return _sortOrder;
	}

	bool UIBase::GetIsHovering()
	{
		return _isHovering;
	}

	bool UIBase::GetIsClicked()
	{
		return _isClicked;
	}

	bool UIBase::GetIsGrabbing()
	{
		return _isGrabbing;
	}

	bool UIBase::IsIgnoreFocused()
	{
		return _ignoreFocus;
	}

	void UIBase::SetIsHovering(bool isHovering)
	{
		_isHovering = isHovering;
	}

	void UIBase::SetIsClicked(bool isClicked)
	{
		_isClicked = isClicked;
	}

	void UIBase::SetIsGrabbing(bool isGrabbing)
	{
		_isGrabbing = isGrabbing;
	}

	void UIBase::SetIsIgnoreFocus(bool isIgnore)
	{
		_ignoreFocus = isIgnore;
	}

	float UIBase::GetLeft()
	{
		auto b = GetTransform()->GetPosition().x;
		auto c = _sketchable->GetWidth() * GetTransform()->GetScale().x / 2;

		auto a = GetTransform()->GetPosition().x -
			(_sketchable->GetWidth() * GetTransform()->GetScale().x / 2);

		return {};
	}

	float UIBase::GetRight()
	{
		return GetTransform()->GetPosition().x +
			(_sketchable->GetWidth() * GetTransform()->GetScale().x / 2);
	}

	float UIBase::GetTop()
	{
		return GetTransform()->GetPosition().y -
			(_sketchable->GetHeight() * GetTransform()->GetScale().y / 2);
	}

	float UIBase::GetBottom()
	{
		return GetTransform()->GetPosition().y +
			(_sketchable->GetHeight() * GetTransform()->GetScale().y / 2);
	}

}
