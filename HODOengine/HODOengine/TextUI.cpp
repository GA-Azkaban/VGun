#include "TextUI.h"
#include "Transform.h"

namespace HDData
{
	TextUI::TextUI()
		: _textUI(nullptr //RenderSystem.CreateImageUI()
		)
	{

	}

	void TextUI::Update()
	{
		_textUI->SetWorldTM(GetTransform()->GetWorldTM());
	}

	void TextUI::SetActive(bool active)
	{
		_textUI->SetActive(active);
	}

	void TextUI::SetScreenSpace()
	{
		_textUI->SetScereenSpace();
	}

	void TextUI::SetWorldSpace()
	{
		_textUI->SetWorldSpace();
	}

	void TextUI::SetText(TCHAR* text)
	{
		_textUI->SetText(text);
	}
	
	std::string TextUI::GetText()
	{
		return _textUI->GetText();
	}

}