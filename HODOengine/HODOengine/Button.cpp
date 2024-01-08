#include "Button.h"
#include "ImageUI.h"
#include "TextUI.h"
#include "GameObject.h"

namespace HDData
{
	Button::Button()
	{
		auto obj = GetGameObject();
		obj->AddComponent<ImageUI>();
		obj->AddComponent<TextUI>();
	}

	void Button::SetOnClickEvent(std::function<void()> event)
	{
		_onClickEvent = event;
	}

	std::function<void()> Button::GetClickEvent() 
	{
		return _onClickEvent;
	}

}

