#pragma once
#include "UIBase.h"
#include "..\\HODO3DGraphicsInterface\\ISketchableImage.h"

namespace HDData
{
	class ImageUI : public UIBase
	{
	public:
		ImageUI();

		void SetActive(bool active);
		void SetScreenSpace();
		void SetWorldSpace();
		void SetImage(const char* fileName);

	private:
		HDEngine::ISketchableImage* _imageUI;
	};
}

