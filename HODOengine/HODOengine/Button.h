#pragma once
#include <functional>

#include "UIBase.h"
#include "ImageUI.h"
#include "..\\HODO3DGraphicsInterface\\ISketchableImage.h"

namespace HDData
{
	class HODO_API Button : public Component
	{
	public:
		Button();

	public:
		void Start() override;
		void Update() override;

	public:
		void SetActive(bool active);
		void SetImage(const char* fileName);

		void SetOnClickEvent(std::function<void()> event);
		std::function<void()> GetClickEvent();
		void CallClickEvent();

		void SetButtonComp(ImageUI* comp) { _button = comp; }
		void SetSortOrder(float ord) { _button->SetSortOrder(ord); }
		void SetIgnoreFocus(bool isIgnore);

		void SetColor(DirectX::FXMVECTOR color);
		void SetDefaultColor(DirectX::FXMVECTOR color);
		void ReturnDefaultColor();

		void ChangeScale(float x, float y);

		ImageUI* GetButtonComp();

		// FadeIn & out
		void FadeIn(float time);
		void FadeOut(float time);
		bool GetFadeMode();
		bool SetFadeMode(bool SetFade, float time);

	private:
		ImageUI* _button;

		std::function<void()> _onClickEvent;
	};
}


