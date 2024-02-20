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

	public:
		void Start() override;
		void Update() override;
		void OnEnable() override;
		void OnDisable() override;

		std::string GetCurrentText();

		// 자식 객체 설정
		void SetBoxComp(ImageUI* comp);
		void SetCursorComp(ImageUI* comp);
		void SetTextComp(TextUI* comp);

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


