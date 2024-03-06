#pragma once
#include <vector>
#include <memory>
#include <string>

#include "Component.h"

namespace HDData
{
	class ImageUI;
	class TextUI;

	class HODO_API SlideBoxUI : public Component
	{
	public:
		SlideBoxUI();

	public:
		void Start() override;
		void Update() override;

		// 전체 오브젝트 활성화 여부
		void SetActive(bool active);
		void SetScreenSpace();
		void SetWorldSpace();

		// 텍스트 내용
		void SetText(const std::string& str);
		std::string GetText();
		void AddTextList(const std::string str);
		void ChangeText(bool isRight);

		// 화살표 이미지
		void SetLeftArrowImage(const char* fileName);
		void SetRightArrowImage(const char* fileName);

	private:
		std::vector<std::string> _values;

		ImageUI* _leftArrow;
		ImageUI* _rightArrow;
		TextUI* _valueText;

		bool _isRight;
	};

}


