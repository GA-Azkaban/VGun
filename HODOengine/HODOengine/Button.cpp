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
		: _buttonUI(HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateText())
	{
		_sketchable.push_back(_buttonUI);
	}

	void Button::Start()
	{
		_buttonUI->SetText("button");
	}

	void Button::Update()
	{
		if (HDEngine::InputSystem::Instance().GetMouseDown(MOUSE_LEFT))
		{
			_onClickEvent;
		}
	}

	void Button::SetActive(bool active)
	{
		_buttonUI->SetActive(active);
	}

	void Button::SetScreenSpace()
	{

	}

	void Button::SetWorldSpace()
	{

	}

	void Button::SetImage(const char* fileName)
	{
		
	}

	void Button::SetText(const std::string& str)
	{
		_buttonUI->SetText(str);
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

