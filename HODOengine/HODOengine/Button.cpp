#include "Button.h"
#include "ImageUI.h"
#include "TextUI.h"
#include "GameObject.h"
#include "GraphicsObjFactory.h"
#include "RenderSystem.h"
#include "EventSystem.h"
#include "InputSystem.h"
#include "InputData.h"

namespace HDData
{
	Button::Button()
	{

	}

	void Button::Start()
	{

	}

	void Button::Update()
	{
		if (_button->GetIsClicked())
		{
			CallClickEvent();
		}
	}

	void Button::SetActive(bool active)
	{
		_button->SetActive(active);
	}

	void Button::SetImage(const char* fileName)
	{
		_button->SetImage(fileName);
	}

	void Button::SetOnClickEvent(std::function<void()> event)
	{
		_onClickEvent = event;
	}

	std::function<void()> Button::GetClickEvent() 
	{
		return _onClickEvent;
	}

	void Button::CallClickEvent()
	{
		_onClickEvent();
	}

	void Button::SetIgnoreFocus(bool isIgnore)
	{
		_button->SetIsIgnoreFocus(isIgnore);
	}

	HDData::ImageUI* Button::GetButtonComp()
	{
		return _button;
	}

	void Button::FadeIn()
	{
		_button->FadeIn();
	}

	void Button::FadeOut()
	{
		_button->FadeOut();
	}

	bool Button::GetFadeMode()
	{
		return _button->GetFadeMode();
	}

	bool Button::SetFadeMode(bool SetFade)
	{
		return _button->SetFadeMode(SetFade);
	}

}

