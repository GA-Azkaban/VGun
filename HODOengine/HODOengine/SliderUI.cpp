#include "SliderUI.h"
#include "GameObject.h"
#include "ImageUI.h"
#include "GraphicsObjFactory.h"
#include "RenderSystem.h"
#include "InputSystem.h"

#include <string>

namespace HDData
{
	SliderUI::SliderUI()
		: _sliderBar(HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateImage()),
		_sliderPoint(HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateImage()),
		_valueText(HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateText()),
		_inputSystem(HDEngine::InputSystem::Instance()),
		_value(50)
	{
		HDEngine::RenderSystem::Instance().PushSketchComponent(this);
		_sketchable.push_back(_sliderBar);
		_sketchable.push_back(_sliderPoint);
		_sketchable.push_back(_valueText);
	}

	void SliderUI::Start()
	{
		_valueText->SetText(std::to_string(_value));
		_sliderPoint->SetImage("../Resources/circle.png");
		_sliderPoint->ChangeScale(0.05f, 0.05f);
		_sliderBar->SetImage("../Resources/bar.png");
		_sliderBar->ChangeScale(1.f, 0.2f);
	}

	void SliderUI::Update()
	{
		auto now = _sliderPoint->GetScreenSpacePositionX();

		if (_inputSystem.GetMouse(MOUSE_LEFT))
		{
			_sliderPoint->SetScreenSpacePosition(_inputSystem.GetMousePosition().x - 50, GetTransform()->GetWorldPosition().y);
			_valueText->SetScreenSpacePosition(_inputSystem.GetMousePosition().x - 50, GetTransform()->GetWorldPosition().y - 30);

			// TODO) 길이를 가져와서 값을 제한하고 퍼센티지로 계산해야 함
		}
	}

	void SliderUI::SetActive(bool active)
	{
		_valueText->SetActive(active);
		_sliderBar->SetActive(active);
		_sliderPoint->SetActive(active);
	}

	void SliderUI::SetScreenSpace()
	{
		_valueText->SetScereenSpace();
		_sliderBar->SetScereenSpace();
		_sliderPoint->SetScereenSpace();
	}

	void SliderUI::SetWorldSpace()
	{
		_valueText->SetWorldSpace();
		_sliderBar->SetWorldSpace();
		_sliderPoint->SetWorldSpace();
	}

	void SliderUI::SetText(const std::string& str)
	{
		_valueText->SetText(str);
	}

	std::string SliderUI::GetText()
	{
		return _valueText->GetText();
	}

	void SliderUI::SetDefaultValue(int val)
	{
		_value = val;
	}

	int SliderUI::GetDefaultValue()
	{
		return _value;
	}

	void SliderUI::SetSliderbarImage(const char* fileName)
	{
		_sliderBar->SetImage(fileName);
	}

	void SliderUI::SetSliderpointImage(const char* fileName)
	{
		_sliderPoint->SetImage(fileName);
	}

}
