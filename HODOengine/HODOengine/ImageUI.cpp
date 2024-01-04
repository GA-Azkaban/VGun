#include "ImageUI.h"
#include "Transform.h"

namespace HDData
{
	ImageUI::ImageUI()
		: _imageUI(nullptr //RenderSystem.CreateImageUI()
		)
	{

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

	void ImageUI::SetImage(const char* fileName)
	{
		_imageUI->SetImage(fileName);
	}
}