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
		
	private:
		std::unique_ptr<HDEngine::ISketchableImage> _imageUI;
	};
}

