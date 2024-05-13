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
		_textOriginPos = GetTextUI()->GetTransform()->GetPosition().x + 5;
		GetBackgroundImage()->SetActive(true);
		GetCursorImage()->SetActive(false);
	}

	void TextInputBoxUI::Update()
	{
		if (GetBackgroundImage()->GetIsClicked())
		{
			GetCursorImage()->SetActive(true);
			_inputReady = true;
		}

		if (_inputReady)
		{
			_blankTime += HDEngine::TimeSystem::Instance().GetDeltaTime();

			if (_blankTime > 0.5)
			{
				_blankTime = 0;
				_isCursorOn = !_isCursorOn;
				GetCursorImage()->SetActive(_isCursorOn);
			}

			for (int i = 0; i < 0x37; ++i)
			{
				if (HDEngine::InputSystem::Instance().GetKeyDown(i))
				{
					if (newVal.size() < 20)
					{
						if (i != DIK_ESCAPE && i != DIK_TAB && 
							i != DIK_BACK && i != DIK_RETURN &&
							i != DIK_LCONTROL && i != DIK_LSHIFT && 
							i != DIK_RSHIFT)
						{
							newVal += HDEngine::InputSystem::Instance().GetInputText(i);
							GetTextUI()->SetText(newVal);
							GetCursorImage()->GetTransform()->SetPosition(_textOriginPos + GetTextUI()->_sketchable->GetWidth() / 2, GetTextUI()->GetTop() + GetTextUI()->_sketchable->GetHeight() / 2, 0.f);
						}
					}
					if (i == DIK_BACKSPACE && newVal.size() > 0)
					{
						newVal.pop_back();
						GetTextUI()->SetText(newVal);
						GetCursorImage()->GetTransform()->SetPosition(_textOriginPos + GetTextUI()->_sketchable->GetWidth() / 2 - 5, GetTextUI()->GetTop() + GetTextUI()->_sketchable->GetHeight() / 2, 0.f);
					}
				}
			}

			if (HDEngine::InputSystem::Instance().GetMouseDown(MOUSE_LEFT) && !GetBackgroundImage()->GetIsClicked())
			{
				GetCursorImage()->SetActive(false);
				_inputReady = false;
			}
		}
	}

	std::string TextInputBoxUI::GetCurrentText()
	{
		return newVal;
	}

	void TextInputBoxUI::SetSortOrder(float value)
	{
		_background->SetSortOrder(value);
		_cursor->SetSortOrder(value + 0.1f);
		_text->SetSortOrder(value + 0.2f);
	}

}
