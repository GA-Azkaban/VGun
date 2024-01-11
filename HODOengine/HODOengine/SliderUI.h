#pragma once
#include "UIBase.h"
#include "..\\HODO3DGraphicsInterface\\ISketchableImage.h"
#include "..\\HODO3DGraphicsInterface\\ISketchableText.h"

namespace HDData
{
	class SliderUI : public UIBase
	{
	public:
		SliderUI();

	public:
		// 전체 슬라이더 활성화 여부
		void SetActive(bool active);

		// 전체 슬라이더 위치
		void SetScreenSpace();
		void SetWorldSpace();

		// 슬라이더 방향
		void SetIsHorizontal(bool isHorizontal);
		bool GetIsHorizontal();

		// 텍스트 내용
		void SetText(const std::string& str);
		std::string GetText();
		void SetSliderbarImage(const char* fileName);
		void SetSliderpointImage(const char* fileName);

	private:
		// 비주얼라이즈를 위한 그래픽스 객체들
		HDEngine::ISketchableText* _valueText;
		HDEngine::ISketchableImage* _sliderBar;
		HDEngine::ISketchableImage* _sliderPoint;

		// 슬라이더가 가로 방향인지 세로 방향인지
		bool _isHorizontal;
		
		// 슬라이더의 최대 최소 위치
		int _min;
		int _max;
	};
}


