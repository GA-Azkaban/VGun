#include "TextUI.h"
#include "Transform.h"
#include "GraphicsObjFactory.h"
#include "RenderSystem.h"

namespace HDData
{
	TextUI::TextUI()
		: _textUI(HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateText())
	{
		HDEngine::RenderSystem::Instance().PushSketchComponent(this);
		_sketchable = _textUI;
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