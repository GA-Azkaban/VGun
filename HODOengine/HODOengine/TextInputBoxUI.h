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

	private:
		ImageUI* _background;
		ImageUI* _cursor;
		TextUI* _text;

		bool _isCursorOn;
		float _blankTime;
		const char* _value[];
	};

}


