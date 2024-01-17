#include "ToggleUI.h"
#include "RenderSystem.h"
#include "GraphicsObjFactory.h"
#include "InputSystem.h"

HDData::ToggleUI::ToggleUI()
	: _toggleImage(HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateImage()),
	_inputSystem(HDEngine::InputSystem::Instance()),
	_isToggleOn(true)
{
	HDEngine::RenderSystem::Instance().PushSketchComponent(this);
	_sketchable = _toggleImage;
}

void HDData::ToggleUI::Start()
{
	_toggleOffImage = "Mute.png";
	_toggleOnImage = "Sound.png";
}

void HDData::ToggleUI::Update()
{
	if (CheckIsClicked() && _inputSystem.GetMouseDown(MOUSE_LEFT))
	{
		if (_isToggleOn)
		{
			_toggleImage->SetImage(_toggleOnImage);
		}
		else
		{
			_toggleImage->SetImage(_toggleOffImage);
		}
	}

}

void HDData::ToggleUI::SetActive(bool active)
{
	_toggleImage->SetActive(active);
}

void HDData::ToggleUI::SetScreenSpace()
{
	_toggleImage->SetScreenSpace();
}

void HDData::ToggleUI::SetWorldSpace()
{
	_toggleImage->SetWorldSpace();
}

void HDData::ToggleUI::SetToggleOnImage(const char* fileName)
{
	_toggleOnImage = fileName;
}

void HDData::ToggleUI::SetToggleOffImage(const char* fileName)
{
	_toggleOffImage = fileName;
}

bool HDData::ToggleUI::CheckIsClicked()
{
	if (_inputSystem.GetMousePosition().x > _toggleImage->GetScreenSpacePositionX() &&
		_inputSystem.GetMousePosition().y > _toggleImage->GetScreenSpacePositionY())
	{
		return true;
	}
}

