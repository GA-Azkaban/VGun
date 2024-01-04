#pragma once
#include "UIBase.h"
#include "..\\HODO3DGraphicsInterface\\ISketchableText.h"
#include <memory>
#include <string>

namespace HDData
{
	class TextUI : public UIBase
	{
	public:
		TextUI();

		void SetActive(bool active);
		void SetScreenSpace();
		void SetWorldSpace();
		void SetText(TCHAR* text);
		std::string GetText();

	private:
		HDEngine::ISketchableText* _textUI;
	};
}

