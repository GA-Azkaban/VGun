#include "ImageUI.h"
#include "Transform.h"
#include "GraphicsObjFactory.h"
#include "RenderSystem.h"

namespace HDData
{
	ImageUI::ImageUI()
		: _imageUI(HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateImage())
	{
		HDEngine::RenderSystem::Instance().PushSketchComponent(this);
		_sketchable = _imageUI;
	}

	void ImageUI::SetActive(bool active)
	{
		_imageUI->SetActive(active);
	}

	void ImageUI::SetScreenSpace()
	{
		_imageUI->SetScereenSpace();
	}

	void ImageUI::SetWorldSpace()
	{
		_imageUI->SetWorldSpace();
	}

	void ImageUI::SetImage(const std::string& fileName)
	{
		_imageUI->SetImage(fileName);
	}
}