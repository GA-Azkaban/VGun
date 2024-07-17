#pragma once
#include "UIBase.h"
#include "dllExporter.h"
#include "..\\HODO3DGraphicsInterface\\ISketchableImage.h"

namespace HDData
{
	class HODO_API ImageUI : public UIBase
	{
	public:
		ImageUI();

		void SetActive(bool active);
		void SetScreenSpace();
		void SetWorldSpace();
		void SetImage(const std::string& fileName);
		
		float GetScreenSpacePositionX();
		float GetScreenSpacePositionY();

		DirectX::FXMVECTOR SetColor(DirectX::FXMVECTOR color);
		void SetDefaultColor(DirectX::FXMVECTOR color);
		void RetunDefaultColor();

		float GetImageWidth();
		float GetImageHeight();

		void OnClickEvent() override;

		void ChangeScale(float x, float y);

		void SetAngle(float angle);
		void SetOrigin(float x, float y);
		void SetCenter(float x, float y);

		Vector2 GetImageScale();

		// fade In & out
		void FadeIn(float time);
		void FadeOut(float time);
		bool GetComplete();
		bool GetFadeMode();
		bool SetFadeMode(bool SetFade,float time);

	private:
		HDEngine::ISketchableImage* _imageUI;
	};
}

