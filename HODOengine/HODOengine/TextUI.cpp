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
}