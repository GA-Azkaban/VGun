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
		_textOriginPos = _text->GetTransform()->GetPosition().x + 5;
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

		if (_inputReady)
		{
			_blankTime += HDEngine::TimeSystem::Instance().GetDeltaTime();

			if (_blankTime > 0.5)
			{
				_blankTime = 0;
				_isCursorOn = !_isCursorOn;
				_cursor->SetActive(_isCursorOn);
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
							_text->SetText(newVal);
							_cursor->GetTransform()->SetPosition(_textOriginPos + _text->_sketchable->GetWidth() / 2, _text->GetTop() + _text->_sketchable->GetHeight() / 2, 0.f);
						}
					}
					if (i == DIK_BACKSPACE && newVal.size() > 0)
					{
						newVal.pop_back();
						_text->SetText(newVal);
						_cursor->GetTransform()->SetPosition(_textOriginPos + _text->_sketchable->GetWidth() / 2 - 5, _text->GetTop() + _text->_sketchable->GetHeight() / 2, 0.f);
					}
				}
			}

			if (HDEngine::InputSystem::Instance().GetMouseDown(MOUSE_LEFT) && !_background->GetIsClicked())
			{
				_cursor->SetActive(false);
				_inputReady = false;
			}
		}
	}

	void TextInputBoxUI::OnDisable()
	{
		_text->SetText("");
	}

	std::string TextInputBoxUI::GetCurrentText()
	{
		return newVal;
	}

	void TextInputBoxUI::SetBoxComp(ImageUI* comp)
	{
		_background = comp;
	}

	void TextInputBoxUI::SetCursorComp(ImageUI* comp)
	{
		_cursor = comp;
	}

	void TextInputBoxUI::SetTextComp(TextUI* comp)
	{
		_text = comp;
	}

	void TextInputBoxUI::SetSortOrder(float ord)
	{
		_background->SetSortOrder(ord);
		_cursor->SetSortOrder(ord);
		_text->SetSortOrder(ord);
	}

}
