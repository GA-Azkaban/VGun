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

	private:
		HDEngine::ISketchableImage* _imageUI;
	};
}

