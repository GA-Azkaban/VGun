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
		_value(nullptr)
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
			}
		}

		_min = GetTransform()->GetWorldPosition().x - (_background->GetImageWidth() / 2);
		_max = GetTransform()->GetWorldPosition().x + _background->GetImageWidth() / 2;
	}

	void SliderUI::Update()
	{
		if (_background->GetIsClicked() == true)
		{
			auto mouseX = HDEngine::InputSystem::Instance().GetMousePosition().x;
			auto newValue = ((_handle->GetTransform()->GetWorldPosition().x - _background->GetLeft()) / _background->GetImageWidth()) * 100;

			_handle->GetTransform()->SetWorldPosition(mouseX, _background->GetTransform()->GetLocalPosition().y, 0);
			_value->GetTransform()->SetWorldPosition(mouseX, _handle->GetTransform()->GetLocalPosition().y - 50, 0);
			_value->SetText(std::to_string((int)newValue));

			/*if (mouseX > _max)
			{
				_handle->GetTransform()->SetWorldPosition(_max, _handle->GetTransform()->GetLocalPosition().y, GetTransform()->GetWorldPosition().z);
				_value->GetTransform()->SetWorldPosition(_max, _handle->GetTransform()->GetLocalPosition().y - 30, GetTransform()->GetWorldPosition().z);
				_value->SetText("100");
			}
			if (mouseX < _min)
			{
				_handle->GetTransform()->SetWorldPosition(_min, _handle->GetTransform()->GetLocalPosition().y, GetTransform()->GetWorldPosition().z);
				_value->GetTransform()->SetWorldPosition(_min, _handle->GetTransform()->GetLocalPosition().y - 30, GetTransform()->GetWorldPosition().z);
				_value->SetText("0");
			}*/
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

	void SliderUI::SetValueText(std::string val)
	{
		_value->SetText(val);
	}

	float SliderUI::GetValueText()
	{
		return std::stof(_value->GetText());
	}

}
