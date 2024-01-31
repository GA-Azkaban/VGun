#pragma once
#include "Component.h"

namespace HDData
{
	class ImageUI;
	class TextUI;

	class TextInputBoxUI : public Component
	{
	public:
		TextInputBoxUI();

	public:
		void Start() override;
		void Update() override;

		std::string GetCurrentText();

	private:
		ImageUI* _background;
		ImageUI* _cursor;
		TextUI* _text; 

		bool _inputReady;
		bool _isCursorOn;
		float _blankTime;
		float _textOriginPos;

		std::string newVal;
		
		bool _isSecret;
	};

}


