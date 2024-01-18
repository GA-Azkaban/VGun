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
		: _buttonUI(HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateImage())
	{
		HDEngine::RenderSystem::Instance().PushSketchComponent(this);
		_sketchable = _buttonUI;
	}

	void Button::Start()
	{

	}

	void Button::Update()
	{
		if (GetIsClicked() == true)
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
		_buttonUI->SetImage(fileName);
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

}

