#include "GameObject.h"
#include "TextUI.h"
#include "ImageUI.h"
#include "InputData.h"
#include "InputSystem.h"
#include "EventSystem.h"
#include "RenderSystem.h"
#include "GraphicsObjFactory.h"
#include "Transform.h"
#include "SlideBoxUI.h"


namespace HDData
{

	SlideBoxUI::SlideBoxUI()
	{

	}

	void SlideBoxUI::Start()
	{
		//_leftArrow->GetTransform()->SetLocalPosition(-0.5f, 0.f, 0.f);
		//_rightArrow->GetTransform()->SetLocalPosition(0.5f, 0.f, 0.f);
	}

	void SlideBoxUI::Update()
	{
		if (_leftArrow->GetIsClicked())
		{
			ChangeText(false);
		}
		if (_rightArrow->GetIsClicked())
		{
			ChangeText(true);
		}
	}

	void SlideBoxUI::SetActive(bool active)
	{
		_leftArrow->SetActive(active);
		_rightArrow->SetActive(active);
		_valueText->SetActive(active);
	}

	void SlideBoxUI::SetScreenSpace()
	{
		_leftArrow->SetScreenSpace();
		_rightArrow->SetScreenSpace();
		_valueText->SetScreenSpace();
	}

	void SlideBoxUI::SetWorldSpace()
	{
		_leftArrow->SetWorldSpace();
		_rightArrow->SetWorldSpace();
		_valueText->SetWorldSpace();
	}

	void SlideBoxUI::SetText(const std::string& str)
	{
		_valueText->SetText(str);
	}

	std::string SlideBoxUI::GetText()
	{
		return _valueText->GetText();
	}

	void SlideBoxUI::AddTextList(const std::string str)
	{
		_values.push_back(str);
	}

	void SlideBoxUI::ChangeText(bool isRight)
	{
		auto now = std::find(_values.begin(), _values.end(), _valueText->GetText());

		if (now != _values.end())
		{
			if (isRight == true)
			{
				if (std::next(now) != _values.end())
				{
					_valueText->SetText(*(std::next(now)));
				}
				else
				{
					_valueText->SetText(*(_values.begin()));
				}
			}
			else
			{
				if (now != _values.begin())
				{
					_valueText->SetText(*(std::prev(now)));
				}
				else
				{
					_valueText->SetText(*(std::prev(_values.end())));
				}
			}
		}
	}

	void SlideBoxUI::SetLeftArrowImage(const char* fileName)
	{
		_leftArrow->SetImage(fileName);
	}

	void SlideBoxUI::SetRightArrowImage(const char* fileName)
	{
		_rightArrow->SetImage(fileName);
	}

	void SlideBoxUI::SetLeftComp(ImageUI* comp)
	{
		_leftArrow = comp;
	}

	void SlideBoxUI::SetRightComp(ImageUI* comp)
	{
		_rightArrow = comp;
	}

	void SlideBoxUI::SetValueComp(TextUI* comp)
	{
		_valueText = comp;
	}

}
