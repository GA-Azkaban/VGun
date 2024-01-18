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
		auto test = _background->GetImageWidth();
		_min = GetTransform()->GetWorldPosition().x - (_background->GetImageWidth() / 2);
		_max = GetTransform()->GetWorldPosition().x + _background->GetImageWidth() / 2;
	}

	void SliderUI::Update()
	{
		/*if (_handle->GetIsClicked() == true) _isGrabHandle = true;

		if (_isGrabHandle == true)
		{
			auto newValue = ((_handle->GetScreenSpacePositionX() - _background->GetScreenSpacePositionX()) / _background->GetImageWidth()) * 100;

			_handle->GetTransform()->SetWorldPosition(mouseX - _handle->GetImageWidth(), _handle->GetTransform()->GetWorldPosition().y, GetTransform()->GetWorldPosition().z);
			_value->GetTransform()->SetWorldPosition(mouseX - _handle->GetImageWidth(), _handle->GetTransform()->GetWorldPosition().y - 30, GetTransform()->GetWorldPosition().z);
			_value->SetText(std::to_string((int)newValue));

			if (mouseX > _max)
			{
				_handle->GetTransform()->SetWorldPosition(_max, _handle->GetTransform()->GetWorldPosition().y, GetTransform()->GetWorldPosition().z);
				_value->GetTransform()->SetWorldPosition(_max, _handle->GetTransform()->GetWorldPosition().y - 30, GetTransform()->GetWorldPosition().z);
				_value->SetText("100");
			}
			if (mouseX < _min)
			{
				_handle->GetTransform()->SetWorldPosition(_min, _handle->GetTransform()->GetWorldPosition().y, GetTransform()->GetWorldPosition().z);
				_value->GetTransform()->SetWorldPosition(_min, _handle->GetTransform()->GetWorldPosition().y - 30, GetTransform()->GetWorldPosition().z);
				_value->SetText("0");
			}

			if (!_handle->GetIsClicked()) _isGrabHandle = false;
		}*/
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
