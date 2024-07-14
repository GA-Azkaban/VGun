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
		: _onClickEvent(nullptr)
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
		if (_onClickEvent == nullptr) return;

		_onClickEvent();
	}

	void Button::SetIgnoreFocus(bool isIgnore)
	{
		_button->SetIsIgnoreFocus(isIgnore);
	}

	void Button::SetColor(DirectX::FXMVECTOR color)
	{
		_button->SetColor(color);
	}

	void Button::SetDefaultColor(DirectX::FXMVECTOR color)
	{
		_button->SetDefaultColor(color);
	}

	void Button::ReturnDefaultColor()
	{
		_button->RetunDefaultColor();
	}

	void Button::ChangeScale(float x, float y)
	{
		_button->ChangeScale(x, y);
	}

	HDData::ImageUI* Button::GetButtonComp()
	{
		return _button;
	}

	void Button::FadeIn(float time)
	{
		_button->FadeIn(time);
	}

	void Button::FadeOut(float time)
	{
		_button->FadeOut(time);
	}

	bool Button::GetFadeMode()
	{
		return _button->GetFadeMode();
	}

	bool Button::SetFadeMode(bool SetFade, float time)
	{
		return _button->SetFadeMode(SetFade,time);
	}

}

