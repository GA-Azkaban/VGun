#include "UIBase.h"
#include "Transform.h"
#include "InputSystem.h"
#include "GameObject.h"

namespace HDData
{
	UIBase::UIBase()
		:_sketchable(NULL),
		_sortOrder(0.1f)
	{

	}

	bool UIBase::CheckFocus()
	{
		if (_ignoreFocus)
		{
			return false;
		}

		Vector2 mouse = HDEngine::InputSystem::Instance().GetMousePosition();

		if (mouse.x > GetLeft() &&
			mouse.y > GetTop() &&
			mouse.x < GetRight() &&
			mouse.y < GetBottom())
		{
			return true;
		}

		return false;
	}

	void UIBase::OnEnable()
	{
		_sketchable->SetActive(true);
	}

	void UIBase::OnDisable()
	{
		_sketchable->SetActive(false);
	}

	void UIBase::Update()
	{
		_sketchable->SetWorldTM(GetTransform()->GetWorldTM());
	}

	float UIBase::GetSortOrder() const
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

	void UIBase::SetSortOrder(float orderNum)
	{
		_sortOrder = orderNum;
		_sketchable->SetSortOrder(orderNum);
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
		return GetTransform()->GetPosition().x -
			(_sketchable->GetWidth() * GetTransform()->GetScale().x / 2);
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

	void UIBase::FadeIn(float time)
	{
		_sketchable->FadeIn(time);
	}

	void UIBase::FadeOut(float time)
	{
		_sketchable->FadeOut(time);
	}

}
