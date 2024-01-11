#pragma once
#include <functional>

#include "UIBase.h"
#include "..\\HODO3DGraphicsInterface\\ISketchableImage.h"
#include "..\\HODO3DGraphicsInterface\\ISketchableText.h"

namespace HDData
{
	class Button : public UIBase
	{
	public:
		Button();

		void Start() override;
		void Update() override;

	public:
		void SetActive(bool active);
		void SetScreenSpace();
		void SetWorldSpace();
		void SetImage(const char* fileName);
		void SetText(const std::string& str);

		void SetOnClickEvent(std::function<void()> event);
		std::function<void()> GetClickEvent();

	private:
		HDEngine::ISketchableText* _buttonUI;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
		std::function<void()> _onClickEvent;

	};
}


