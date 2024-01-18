//#pragma once
//#include <vector>
//#include <memory>
//#include <string>
//
//#include "UIBase.h"
//#include "..\\HODO3DGraphicsInterface\\ISketchableImage.h"
//#include "..\\HODO3DGraphicsInterface\\ISketchableText.h"
//
//namespace HDData
//{
//	class HODO_API SlideBoxUI : public UIBase
//	{
//	public:
//		SlideBoxUI();
//
//	public:
//		void Start() override;
//		void Update() override;
//
//		// 전체 오브젝트 활성화 여부
//		void SetActive(bool active);
//		void SetScreenSpace();
//		void SetWorldSpace();
//
//		// 텍스트 내용
//		void SetText(const std::string& str);
//		std::string GetText();
//		void AddTextList(const std::string str);
//		void ChangeText(bool isRight);
//
//		// 화살표 이미지
//		void SetLeftArrowImage(const char* fileName);
//		void SetRightArrowImage(const char* fileName);
//
//		bool InvokeClick(const float& x, const float& y);
//
//	private:
//		std::vector<std::string> _values;
//
//		UIBase* _leftArrow;
//		UIBase* _rightArrow;
//		UIBase* _valueImage;
//
//		bool _isRight;
//	};
//
//}
//
//
