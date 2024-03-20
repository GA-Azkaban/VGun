#include "SpritePass.h"
#include "QuadBuffer.h"
#include "ResourceManager.h"
#include "ObjectManager.h"
#include "ImageRenderer.h"
#include "TextRenderer.h"

namespace RocketCore::Graphics
{

	SpritePass::SpritePass(QuadBuffer* toneMapBuffer)
		: _toneMapBuffer(toneMapBuffer)
	{
		_spriteBatch = new DirectX::SpriteBatch(ResourceManager::Instance().GetDeviceContext());
	}

	SpritePass::~SpritePass()
	{
		delete _spriteBatch;
		delete _toneMapBuffer;
	}

	void SpritePass::Render()
	{
		_toneMapBuffer->SetRenderTargets();

		_spriteBatch->Begin(DirectX::DX11::SpriteSortMode_FrontToBack);
		RenderImage();
		RenderText();
		_spriteBatch->End();
	}

	void SpritePass::RenderImage()
	{
		for (auto imageRenderer : ObjectManager::Instance().GetImageList())
		{
			imageRenderer->Render(_spriteBatch);
		}
	}

	void SpritePass::RenderText()
	{
		for (auto textRenderer : ObjectManager::Instance().GetTextList())
		{
			textRenderer->Render(_spriteBatch);
		}
	}

}
