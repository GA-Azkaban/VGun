#pragma once
#include "Component.h"

namespace HDEngine
{
	class InputSystem;
}

namespace HDData
{
	class TextUI;
	class ImageUI;

	class HODO_API SliderUI : public Component
	{
	public:
		SliderUI();

	public:
		void Start() override;
		void Update() override;

		// 전체 슬라이더 활성화 여부
		void SetActive(bool active);
		void SetScreenSpace();
		void SetWorldSpace();

		// 슬라이더 내용
		void SetSliderbarImage(const char* fileName);
		void SetSliderFillImage(const char* fileName);
		void SetSliderHandleImage(const char* fileName);

		void SetSortOrder(float value);

		void SetValueText(std::string val);
		float GetValueText();

	private:
		ImageUI* _background;
		ImageUI* _fill;
		ImageUI* _handle;
		TextUI* _value;
		
		// 슬라이더의 최대 최소 위치
		int _min;
		int _max;
		int _defaultValue;
	};
}


