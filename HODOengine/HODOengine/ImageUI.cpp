#include "ImageUI.h"
#include "Transform.h"
#include "GraphicsObjFactory.h"
#include "RenderSystem.h"
#include "GameObject.h"
#include "ToggleUI.h"
#include "Component.h"

namespace HDData
{
	ImageUI::ImageUI()
		: _imageUI(HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateImage())
	{
		//HDEngine::RenderSystem::Instance().PushSketchComponent(this);
		_sketchable = _imageUI;
		_imageUI->SetActive(true);
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

	void ImageUI::SetDefaultColor(DirectX::FXMVECTOR color)
	{
		_imageUI->SetDefalutColor(color);
	}

	void ImageUI::RetunDefaultColor()
	{
		_imageUI->RetunDefalutColor();
	}

	float ImageUI::GetImageWidth()
	{
		return _imageUI->GetWidth();
	}

	float ImageUI::GetImageHeight()
	{
		return _imageUI->GetHeight();
	}

	void ImageUI::OnClickEvent()
	{

	}

	void ImageUI::ChangeScale(float x, float y)
	{
		_imageUI->ChangeScale(x, y);
	}

	void ImageUI::SetAngle(float angle)
	{
		_imageUI->SetAngle(angle);
	}

	void ImageUI::SetOrigin(float x, float y)
	{
		_imageUI->SetOrigin(x, y);
	}

	void ImageUI::SetCenter(float x, float y)
	{
		_imageUI->SetCenter(x, y);
	}

	void ImageUI::FadeIn(float time)
	{
		return _imageUI->FadeIn(time);
	}

	void ImageUI::FadeOut(float time)
	{
		return _imageUI->FadeOut(time);
	}

	bool ImageUI::GetComplete()
	{
		return _imageUI->GetComplete();
	}

	bool ImageUI::GetFadeMode()
	{
		return _imageUI->GetFadeMode();
	}

}
