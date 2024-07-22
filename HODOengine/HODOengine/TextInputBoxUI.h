#pragma once
#include "Component.h"

namespace HDData
{
	class ImageUI;
	class TextUI;

	class HODO_API TextInputBoxUI : public Component
	{
	public:
		TextInputBoxUI();
		void Start() override;
		void Update() override;

		std::string GetCurrentText();

		HDData::ImageUI* GetBackgroundImage() const { return _background; }
		HDData::ImageUI* GetCursorImage() const { return _cursor; }
		HDData::TextUI* GetTextUI() const { return _text; }
	public:
		void SetBackgroundImage(HDData::ImageUI* val) { _background = val; }
		void SetCursorImage(HDData::ImageUI* val) { _cursor = val; }
		void SetTextUI(HDData::TextUI* val) { _text = val; }

		void SetOrigin();
		void SetSortOrder(float value);

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


