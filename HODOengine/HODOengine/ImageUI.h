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

		float GetImageWidth();
		float GetImageHeight();

		void ChangeScale(float x, float y);

	private:
		HDEngine::ISketchableImage* _imageUI;
	};
}

