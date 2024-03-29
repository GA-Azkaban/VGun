﻿#pragma once
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

		float GetImageWidth();
		float GetImageHeight();

		void OnClickEvent() override;

		void ChangeScale(float x, float y);

		// fade In & out
		void FadeIn();
		void FadeOut();

	private:
		HDEngine::ISketchableImage* _imageUI;
	};
}

