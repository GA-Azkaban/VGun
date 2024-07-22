#include "SliderUI.h"
#include "GameObject.h"
#include "ImageUI.h"
#include "TextUI.h"
#include "GraphicsObjFactory.h"
#include "RenderSystem.h"
#include "InputSystem.h"

#include <string>

namespace HDData
{
	SliderUI::SliderUI()
		: _min(0), _max(0),
		_background(nullptr),
		_fill(nullptr),
		_handle(nullptr),
		_value(nullptr),
		_defaultValue(0)
	{
	}

	void SliderUI::Start()
	{
		auto childs = this->GetGameObject()->GetChildGameObjects();

		for (const auto& child : childs)
		{
			if (child->GetObjectName() == "bar")
			{
				_background = child->GetComponent<HDData::ImageUI>();
			}
			if (child->GetObjectName() == "fill")
			{
				_fill = child->GetComponent<HDData::ImageUI>();
			}
			if (child->GetObjectName() == "handle")
			{
				_handle = child->GetComponent<HDData::ImageUI>();
			}
			if (child->GetObjectName() == "value")
			{
				_value = child->GetComponent<HDData::TextUI>();
				_value->SetFont("Resources/Font/KRAFTON_25.spriteFont");
			}
		}

		_min = GetTransform()->GetPosition().x - (_background->GetImageWidth() / 2);
		_max = GetTransform()->GetPosition().x + _background->GetImageWidth() / 2;
		_handle->GetTransform()->SetPosition(_max, GetTransform()->GetPosition().y, 0);
		_value->GetTransform()->SetPosition(_background->GetTransform()->GetPosition().x + _background->GetImageWidth() + 1, _handle->GetTransform()->GetPosition().y, 0);
	}

	void SliderUI::Update()
	{
		if (_background->GetIsGrabbing())
		{
			auto mouseX = HDEngine::InputSystem::Instance().GetMousePosition().x;
			auto newValue = ((_handle->GetTransform()->GetPosition().x - _background->GetLeft()) / _background->GetImageWidth()) * 100;

			_handle->GetTransform()->SetPosition(mouseX, _background->GetTransform()->GetPosition().y, 0);
			//_value->GetTransform()->SetPosition(mouseX - 20, _handle->GetTransform()->GetPosition().y - 50, 0);
			_value->SetText(std::to_string((int)newValue));

			if (mouseX > _max)
			{
				_handle->GetTransform()->SetPosition(_max, _handle->GetTransform()->GetPosition().y, 0);
				//_value->GetTransform()->SetPosition(_max - 20, _handle->GetTransform()->GetPosition().y - 50, 0);
				_value->SetText("100");
			}
			if (mouseX < _min)
			{
				_handle->GetTransform()->SetPosition(_min, _handle->GetTransform()->GetPosition().y, 0);
				//_value->GetTransform()->SetPosition(_min - 20, _handle->GetTransform()->GetPosition().y - 50, 0);
				_value->SetText("0");
			}
		}
	}

	void SliderUI::SetActive(bool active)
	{
		_background->SetActive(active);
		_fill->SetActive(active);
		_handle->SetActive(active);
		_value->SetActive(active);
	}

	void SliderUI::SetScreenSpace()
	{
		_background->SetScreenSpace();
		_fill->SetScreenSpace();
		_handle->SetScreenSpace();
		_value->SetScreenSpace();
	}

	void SliderUI::SetWorldSpace()
	{
		_background->SetWorldSpace();
		_fill->SetWorldSpace();
		_handle->SetWorldSpace();
		_value->SetWorldSpace();
	}

	void SliderUI::SetSliderbarImage(const char* fileName)
	{
		_background->SetImage(fileName);
	}

	void SliderUI::SetSliderFillImage(const char* fileName)
	{
		_fill->SetImage(fileName);
	}

	void SliderUI::SetSliderHandleImage(const char* fileName)
	{
		_handle->SetImage(fileName);
	}

	void SliderUI::SetSortOrder(float value)
	{
		_background->SetSortOrder(value);
		_handle->SetSortOrder(value + 0.1f);
		_value->SetSortOrder(value);
	}

	void SliderUI::SetValueText(std::string val)
	{
		_value->SetText(val);
	}

	float SliderUI::GetValueText()
	{
		return std::stof(_value->GetText());
	}

}
