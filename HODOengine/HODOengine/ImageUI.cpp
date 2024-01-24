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
		_imageUI->SetScreenSpace();
	}

	void ImageUI::SetWorldSpace()
	{
		_imageUI->SetWorldSpace();
	}

	void ImageUI::SetImage(const std::string& fileName)
	{
		_imageUI->SetImage(fileName);
	}

	float ImageUI::GetScreenSpacePositionX()
	{
		return _imageUI->GetScreenSpacePositionX();
	}

	float ImageUI::GetScreenSpacePositionY()
	{
		return _imageUI->GetScreenSpacePositionY();
	}

	DirectX::FXMVECTOR ImageUI::SetColor(DirectX::FXMVECTOR color)
	{
		return _imageUI->SetColor(color);
	}

	float ImageUI::GetImageWidth()
	{
		return _imageUI->GetWidth();
	}

	float ImageUI::GetImageHeight()
	{
		return _imageUI->GetHeight();
	}

	void ImageUI::ChangeScale(float x, float y)
	{
		_imageUI->ChangeScale(x, y);
	}

}
