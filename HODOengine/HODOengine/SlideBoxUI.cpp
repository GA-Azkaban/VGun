#include "SlideBoxUI.h"
#include "Transform.h"
#include "GraphicsObjFactory.h"
#include "RenderSystem.h"
#include "EventSystem.h"
#include "InputSystem.h"
#include "InputData.h"


namespace HDData
{

	SlideBoxUI::SlideBoxUI()
		: _valueText(HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateText()),
		_texts(NULL)
	{
		HDEngine::RenderSystem::Instance().PushSketchComponent(this);
		_sketchable = _valueText;
	}

	void SlideBoxUI::Start()
	{

	}

	void SlideBoxUI::Update()
	{
		float x = HDEngine::InputSystem::Instance().GetMousePosition().x;
		float y = HDEngine::InputSystem::Instance().GetMousePosition().y;

		if (InvokeClick(x, y))
		{
			ChangeText(_isRight);
		}
	}

	void SlideBoxUI::SetActive(bool active)
	{
		_valueText->SetActive(active);
		_arrowLeft->SetActive(active);
		_arrowRight->SetActive(active);
	}

	void SlideBoxUI::SetScreenSpace()
	{
		_valueText->SetScereenSpace();
		_arrowLeft->SetScereenSpace();
		_arrowRight->SetScereenSpace();
	}

	void SlideBoxUI::SetWorldSpace()
	{
		_valueText->SetWorldSpace();
		_arrowLeft->SetWorldSpace();
		_arrowRight->SetWorldSpace();
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
		_texts.push_back(str);
	}

	void SlideBoxUI::ChangeText(bool isRight)
	{
		auto now = std::find(_texts.begin(), _texts.end(), _valueText->GetText());

		if (now != _texts.end())
		{
			if (isRight == true)
			{
				if (std::next(now) != _texts.end())
				{
					_valueText->SetText(*(std::next(now)));
				}
				else
				{
					_valueText->SetText(*(_texts.begin()));
				}
			}
			else
			{
				if (now != _texts.begin())
				{
					_valueText->SetText(*(std::prev(now)));
				}
				else
				{
					_valueText->SetText(*(std::prev(_texts.end())));
				}
			}
		}
	}

	void SlideBoxUI::SetLeftArrowImage(const char* fileName)
	{
		_arrowLeft->SetImage(fileName);
	}

	void SlideBoxUI::SetRightArrowImage(const char* fileName)
	{
		_arrowRight->SetImage(fileName);
	}

	bool SlideBoxUI::InvokeClick(const float& x, const float& y)
	{
		if (HDEngine::InputSystem::Instance().GetMouseUp(MOUSE_LEFT))
		{
			if (x <= GetTransform()->GetWorldPosition().x)
			{
				_isRight = false;
				return true;
			}
			else if (x > GetTransform()->GetWorldPosition().x)
			{
				_isRight = true;
				return true;
			}
		}

		return false;
	}

}
