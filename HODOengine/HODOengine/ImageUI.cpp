#include "ImageUI.h"
#include "Transform.h"

namespace HDData
{
	ImageUI::ImageUI()
		: _imageUI(nullptr //RenderSystem.CreateImageUI()
		)
	{

	}

	void ImageUI::Update()
	{
		// World Space에 그릴 때와
		// Screen Space에 그릴 때?
		_imageUI->SetWorldTM(GetTransform()->GetWorldTM());
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