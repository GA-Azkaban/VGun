#pragma once
#include "UIBase.h"
#include "..\\HODO3DGraphicsInterface\\ISketchableImage.h"
#include "..\\HODO3DGraphicsInterface\\ISketchableText.h"

namespace HDEngine
{
	class InputSystem;
}

namespace HDData
{
	class HODO_API SliderUI : public UIBase
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

		// 텍스트 내용
		void SetText(const std::string& str);
		std::string GetText();
		
		void SetDefaultValue(int val);
		void SetValue(int val);
		int GetValue();

		void SetSliderbarImage(const char* fileName);
		void SetSliderpointImage(const char* fileName);

	private:
		HDEngine::InputSystem& _inputSystem;

		// 비주얼라이즈를 위한 그래픽스 객체들
		HDEngine::ISketchableText* _valueText;
		HDEngine::ISketchableImage* _sliderBar;
		HDEngine::ISketchableImage* _sliderPoint;
		
		// 슬라이더의 최대 최소 위치
		int _min = 0;
		int _max = 100;

		// 슬라이더 값
		int _value;
	};
}


