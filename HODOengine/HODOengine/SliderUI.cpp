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
		_value(0.5f)
	{
		HDEngine::RenderSystem::Instance().PushSketchComponent(this);
		_sketchable.push_back(_sliderBar);
		_sketchable.push_back(_sliderPoint);
		_sketchable.push_back(_valueText);
	}

	void SliderUI::Start()
	{
		// 이미지와 위치 세팅
		_sliderPoint->SetImage("circle.png");
		_sliderPoint->ChangeScale(0.05f, 0.05f);
		_valueText->SetText(std::to_string((int)_value));
		_sliderBar->SetImage("bar.png");
		_sliderBar->ChangeScale(0.7f, 0.7f);

		HDMath::HDFLOAT3 transform = GetTransform()->GetWorldPosition();

		_valueText->SetScreenSpacePosition(transform.x, transform.y - 30);

		// 포인터 초기 위치 설정
		auto pos = _sliderBar->GetScreenSpacePositionX() + _sliderBar->GetWidth() * _value;
		_sliderPoint->SetScreenSpacePosition(pos, 50.f);

		// 포인터가 이동할 수 있는 x의 최대 최소값
		_min = _sliderBar->GetScreenSpacePositionX();
		_max = _min + _sliderBar->GetWidth();
	}

	void SliderUI::Update()
	{
		if (_inputSystem.GetMouse(MOUSE_LEFT))
		{
			_sliderPoint->SetScreenSpacePosition(_inputSystem.GetMousePosition().x - _sliderPoint->GetWidth(), GetTransform()->GetWorldPosition().y);
			_valueText->SetScreenSpacePosition(_sliderPoint->GetScreenSpacePositionX(), _sliderPoint->GetScreenSpacePositionY() - 30);

			auto point = _sliderPoint->GetScreenSpacePositionX() - _sliderBar->GetScreenSpacePositionX();
			_value = point / _sliderBar->GetWidth() * 100;
			_valueText->SetText(std::to_string(_value));
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
		_valueText->SetScreenSpace();
		_sliderBar->SetScreenSpace();
		_sliderPoint->SetScreenSpace();
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

	void SliderUI::SetValue(int val)
	{
		_value = val;
	}

	int SliderUI::GetValue()
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
