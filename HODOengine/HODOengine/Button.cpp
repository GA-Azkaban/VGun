﻿#include "Button.h"
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

	void Button::FadeIn()
	{
		
	}

	void Button::FadeOut()
	{

	}

}

