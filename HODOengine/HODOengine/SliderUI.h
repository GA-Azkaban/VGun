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
		
		// 슬라이더 영역 범위 내를 클릭하였는지 체크
		bool CheckMouseClicked();

		// 슬라이더 내용
		void SetSliderbarImage(const char* fileName);
		void SetSliderFillImage(const char* fileName);
		void SetSliderHandleImage(const char* fileName);

		void SetValueText(std::string val);
		float GetValueText();
	
	private:
		HDEngine::InputSystem& _inputSystem;

		HDData::ImageUI* _background;
		HDData::ImageUI* _fill;
		HDData::ImageUI* _handle;
		HDData::TextUI* _value;
		
		// 슬라이더의 최대 최소 위치
		int _min = 0;
		int _max = 100;
		bool _isClicked = false;
	};
}


