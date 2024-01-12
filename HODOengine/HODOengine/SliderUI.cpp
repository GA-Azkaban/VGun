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
		_value(10)
	{
		HDEngine::RenderSystem::Instance().PushSketchComponent(this);
		_sketchable.push_back(_sliderBar);
		_sketchable.push_back(_sliderPoint);
		_sketchable.push_back(_valueText);
	}

	void SliderUI::Start()
	{
		_valueText->SetText(std::to_string(_value));
		_valueText->SetScreenSpacePosition(200, 200);

		_min = GetTransform()->GetWorldPosition().x;
		_max = (_min + 500);
		_sliderPoint->SetImage("../Resources/abcd.jpg");
		_sliderBar->ChangeScale(0.0005f, 0.0005f);
	}

	void SliderUI::Update()
	{
		auto now = _sliderPoint->GetScreenSpacePositionX();

		if (_inputSystem.GetMouse(MOUSE_LEFT))
		{
			if (_sliderBar->GetScreenSpacePositionX() )
			{
				_sliderPoint->SetScreenSpacePosition(_inputSystem.GetMousePosition().x, GetTransform()->GetWorldPosition().y);
			}
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
<<<<<<< HEAD
		_valueText->SetScereenSpace();
		_sliderBar->SetScereenSpace();
		_sliderPoint->SetScereenSpace();
=======
		_valueText->SetScreenSpace();
		_sliderBar->SetScreenSpace();
		_sliderPoint->SetScreenSpace();
>>>>>>> DebugRay
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

	void SliderUI::SetSliderbarImage(const char* fileName)
	{
		_sliderBar->SetImage(fileName);
	}

	void SliderUI::SetSliderpointImage(const char* fileName)
	{
		_sliderPoint->SetImage(fileName);
	}

}
