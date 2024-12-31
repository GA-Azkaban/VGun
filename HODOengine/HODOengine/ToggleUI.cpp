#include "ToggleUI.h"
#include "GameObject.h"
#include "ImageUI.h"
#include "TextUI.h"
#include "GraphicsObjFactory.h"
#include "RenderSystem.h"
#include "InputSystem.h"

HDData::ToggleUI::ToggleUI()
	: _isToggleOn(false),
	_toggleOn(nullptr), _toggleOff(nullptr)
{

}

void HDData::ToggleUI::Start()
{
	_toggleOn->SetActive(true);
	_toggleOff->SetActive(false);
}

void HDData::ToggleUI::Update()
{
	if (_toggleOn->GetIsClicked())
	{
		_isToggleOn = !_isToggleOn;
		_toggleOff->SetActive(!_isToggleOn);
		_toggleOn->SetActive(_isToggleOn);

		if (_isToggleOn) { CallToggleOnEvent(); }
		else { CallToggleOffEvent(); }
	}
}

void HDData::ToggleUI::SetToggleOnEvent(std::function<void()> event)
{
	_toggleOnEvent = event;
}

void HDData::ToggleUI::SetToggleOffEvent(std::function<void()> event)
{
	_toggleOffEvent = event;
}

void HDData::ToggleUI::CallToggleOnEvent()
{
	_toggleOnEvent();
}

void HDData::ToggleUI::CallToggleOffEvent()
{
	_toggleOffEvent();
}

void HDData::ToggleUI::SetActive(bool active)
{
	_toggleOn->SetActive(active);
	_toggleOff->SetActive(active);
}

void HDData::ToggleUI::SetScreenSpace()
{
	_toggleOn->SetScreenSpace();
	_toggleOff->SetScreenSpace();
}

void HDData::ToggleUI::SetWorldSpace()
{
	_toggleOn->SetWorldSpace();
	_toggleOff->SetWorldSpace();
}

void HDData::ToggleUI::SetOnComp(ImageUI* comp)
{
	_toggleOn = comp;
}

void HDData::ToggleUI::SetOffComp(ImageUI* comp)
{
	_toggleOff = comp;
}

HDData::ImageUI* HDData::ToggleUI::GetOnComp()
{
	return _toggleOn;
}

HDData::ImageUI* HDData::ToggleUI::GetOffComp()
{
	return _toggleOff;
}

bool HDData::ToggleUI::GetIsOn()
{
	return _isToggleOn;
}

void HDData::ToggleUI::SetSortOrder(float ord)
{
	_toggleOff->SetSortOrder(ord);
	_toggleOn->SetSortOrder(ord);
}


