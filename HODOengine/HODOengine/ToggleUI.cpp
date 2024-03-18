#include "ToggleUI.h"
#include "GameObject.h"
#include "ImageUI.h"
#include "TextUI.h"
#include "GraphicsObjFactory.h"
#include "RenderSystem.h"
#include "InputSystem.h"

HDData::ToggleUI::ToggleUI()
	: _isToggleOn(true),
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
	if (_toggleOff->GetIsClicked())
	{
		_isToggleOn = !_isToggleOn;
		_toggleOff->SetActive(!_isToggleOn);
		_toggleOn->SetActive(_isToggleOn);
	}
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

void HDData::ToggleUI::OnClickEvent()
{

}

void HDData::ToggleUI::SetSortOrder(float ord)
{
	_toggleOff->SetSortOrder(ord);
	_toggleOn->SetSortOrder(ord);
}


