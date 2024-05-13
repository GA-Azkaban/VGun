#include "TextUI.h"
#include "Transform.h"
#include "GraphicsObjFactory.h"
#include "RenderSystem.h"

namespace HDData
{
	TextUI::TextUI()
		: _textUI(HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateText())
	{
		//HDEngine::RenderSystem::Instance().PushSketchComponent(this);
		_sketchable = _textUI;
	}

	void TextUI::SetActive(bool active)
	{
		_textUI->SetActive(active);
	}

	void TextUI::SetScreenSpace()
	{
		_textUI->SetScreenSpace();
	}

	void TextUI::SetWorldSpace()
	{
		_textUI->SetWorldSpace();
	}

	void TextUI::SetFont(const std::string& str)
	{
		_textUI->SetFont(str);
	}

	void TextUI::SetColor(DirectX::FXMVECTOR color)
	{
		_textUI->SetColor(color);
	}

	void TextUI::SetText(const std::string& str)
	{
		_textUI->SetText(str);
	}

	void TextUI::SetDefaultColor(DirectX::FXMVECTOR color)
	{
		_textUI->SetDefaultColor(color);
	}

	void TextUI::ReturnDefaultColor()
	{
		_textUI->ReturnDefaultColor();
	}

	std::string TextUI::GetText()
	{
		return _textUI->GetText();
	}
}
