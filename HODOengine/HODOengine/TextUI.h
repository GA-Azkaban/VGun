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

	private:
		std::unique_ptr<HDEngine::ISketchableText> _textUI;
	};
}

