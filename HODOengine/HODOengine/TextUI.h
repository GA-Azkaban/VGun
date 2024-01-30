#pragma once
#include "dllExporter.h"

#include "UIBase.h"
#include "..\\HODO3DGraphicsInterface\\ISketchableText.h"
#include <memory>
#include <string>

namespace HDData
{
	class HODO_API TextUI : public UIBase
	{
	public:
		TextUI();

		void SetActive(bool active);
		void SetScreenSpace();
		void SetWorldSpace();
		void SetColor(DirectX::FXMVECTOR color);
		void SetText(const std::string& str);
		std::string GetText();

	private:
		HDEngine::ISketchableText* _textUI;
	};
}

