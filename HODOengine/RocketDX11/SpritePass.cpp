#include "SpritePass.h"
#include "QuadBuffer.h"
#include "ResourceManager.h"
#include "ObjectManager.h"
#include "ImageRenderer.h"
#include "TextRenderer.h"
#include "VertexShader.h"
#include "Camera.h"
#include <DirectXMath.h>

namespace RocketCore::Graphics
{

	SpritePass::SpritePass(QuadBuffer* toneMapBuffer)
		: _toneMapBuffer(toneMapBuffer)
	{
		_spriteBatch = new DirectX::SpriteBatch(ResourceManager::Instance().GetDeviceContext());
		_states = new DirectX::CommonStates(ResourceManager::Instance().GetDevice());
	}

	SpritePass::~SpritePass()
	{
		delete _spriteBatch;
		delete _toneMapBuffer;
	}

	void SpritePass::Render()
	{
		_toneMapBuffer->SetRenderTargets();

		DirectX::XMMATRIX view = Camera::GetMainCamera()->GetViewMatrix();
		DirectX::XMMATRIX proj = Camera::GetMainCamera()->GetProjectionMatrix();
		VertexShader* vertexShader = ResourceManager::Instance().GetVertexShader("BillboardVertexShader.cso");
		vertexShader->SetMatrix4x4("viewProjection", XMMatrixTranspose(view * proj));
		//_spriteBatch->Begin(DirectX::DX11::SpriteSortMode_FrontToBack);
		_spriteBatch->Begin(DX11::SpriteSortMode_Deferred, _states->NonPremultiplied());
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
