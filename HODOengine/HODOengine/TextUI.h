#pragma once
#include "Component.h"
#include "..\\HODO3DGraphicsInterface\\ISketchableText.h"
#include <memory>
#include <string>

namespace HDData
{
	class TextUI : public Component
	{
	public:
		TextUI();

		virtual void Update() override;

		void SetActive(bool active);
		void SetScreenSpace();
		void SetWorldSpace();
		void SetText(std::string& str);
		std::string GetText();

	private:
		std::unique_ptr<HDEngine::ISketchableText> _textUI;
	};
}

