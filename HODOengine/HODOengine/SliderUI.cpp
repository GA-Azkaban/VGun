#include "SliderUI.h"
#include "GameObject.h"
#include "ImageUI.h"
#include "GraphicsObjFactory.h"
#include "RenderSystem.h"
#include "InputSystem.h"

namespace HDData
{
	SliderUI::SliderUI()
		: _sliderBar(HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateImage()),
		_sliderPoint(HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateImage()),
		_valueText(HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateText()),
		_inputSystem(HDEngine::InputSystem::Instance())
	{
		HDEngine::RenderSystem::Instance().PushSketchComponent(this);
		_sketchable.push_back(_sliderBar);
		_sketchable.push_back(_sliderPoint);
		_sketchable.push_back(_valueText);
	}

	void SliderUI::Start()
	{
		_min = GetTransform()->GetWorldPosition().x;
		_max = (_min + 500);
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
			//GetTransform()->SetWorldPosition(_inputSystem.GetMousePosition().x, GetTransform()->GetWorldPosition().y, 0);
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

	void SliderUI::SetIsHorizontal(bool isHorizontal)
	{
		_isHorizontal = isHorizontal;
	}

	bool SliderUI::GetIsHorizontal()
	{
		return _isHorizontal;
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
