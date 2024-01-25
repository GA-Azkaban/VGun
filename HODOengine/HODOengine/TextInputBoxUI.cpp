#include "TextInputBoxUI.h"
#include "InputSystem.h"
#include "TimeSystem.h"
#include "GameObject.h"
#include "ImageUI.h"
#include "TextUI.h"


namespace HDData
{

	TextInputBoxUI::TextInputBoxUI()
		: _isCursorOn(false),
		_blankTime(0.f),
		newVal("")
	{

	}

	void TextInputBoxUI::Start()
	{
		auto childs = this->GetGameObject()->GetChildGameObjects();

		for (const auto& child : childs)
		{
			if (child->GetObjectName() == "back")
			{
				_background = child->GetComponent<HDData::ImageUI>();
			}
			if (child->GetObjectName() == "cursor")
			{
				_cursor = child->GetComponent<HDData::ImageUI>();
			}
			if (child->GetObjectName() == "text")
			{
				_text = child->GetComponent<HDData::TextUI>();
			}
		}

		_background->SetActive(true);
		_cursor->SetActive(false);
	}

	void TextInputBoxUI::Update()
	{
		if (_background->GetIsClicked())
		{
			_cursor->SetActive(true);
			_inputReady = true;
		}

		if (_inputReady == true)
		{
			_blankTime += HDEngine::TimeSystem::Instance().GetDeltaTime();

			if (_blankTime > 0.5)
			{
				_blankTime = 0;
				_isCursorOn = !_isCursorOn;
				_cursor->SetActive(_isCursorOn);
			}

			for (int i = 0; i < 256; ++i)
			{
				if (HDEngine::InputSystem::Instance().GetKeyDown(i))
				{
					newVal += HDEngine::InputSystem::Instance().GetInputText(i);
					_text->SetText(newVal);
				}
			}
		}
	}

}
