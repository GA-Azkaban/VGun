#pragma once
#include <functional>

#include "UIBase.h"
#include "..\\HODO3DGraphicsInterface\\ISketchableImage.h"

namespace HDData
{
	class HODO_API Button : public UIBase
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

		void SetOnClickEvent(std::function<void()> event);
		std::function<void()> GetClickEvent();
		void CallClickEvent();

	private:
		HDEngine::ISketchableImage* _buttonUI;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
		std::function<void()> _onClickEvent;

	};
}


