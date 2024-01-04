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
		_imageUI->SetWorldTM(GetTransform()->GetWorldTM());
	}
}