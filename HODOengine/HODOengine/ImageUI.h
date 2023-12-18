#pragma once
#include "Component.h"
#include "..\\HODO3DGraphicsInterface\\ISketchableImage.h"
#include <memory>

namespace HDData
{
	class ImageUI : public Component
	{
	public:
		ImageUI();

		virtual void Update() override;

		void SetActive(bool active);
		void SetScreenSpace();
		void SetWorldSpace();
		void SetImage(const char* fileName);
		
	private:
		std::unique_ptr<HDEngine::ISketchableImage> _imageUI;
	};
}

