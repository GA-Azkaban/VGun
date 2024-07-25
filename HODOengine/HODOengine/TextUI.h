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
		void SetFont(const std::string& str);
		void SetColor(DirectX::FXMVECTOR color);
		void SetAlpha(float alpha);
		void SetText(const std::string& str);

		void SetDefaultColor(DirectX::FXMVECTOR color);
		void ReturnDefaultColor();

		void ChangeScale(float size);

		float GetTextWidth();
		float GetTextHeight();

		std::string GetText();

	private:
		HDEngine::ISketchableText* _textUI;
	};
}

