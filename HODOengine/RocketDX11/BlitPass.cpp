#include "BlitPass.h"
#include "ResourceManager.h"
#include "QuadBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"

namespace RocketCore::Graphics
{
	BlitPass::BlitPass(QuadBuffer* quadBuffer)
		: _quadBuffer(quadBuffer), _resourceManager(ResourceManager::Instance())
	{
		_vertexShader = _resourceManager.GetVertexShader("FullScreenQuadVS.cso");
		_pixelShader = _resourceManager.GetPixelShader("BlitPixelShader.cso");
	}

	BlitPass::~BlitPass()
	{
		delete _quadBuffer;
		delete _vertexShader;
		delete _pixelShader;
	}

	void BlitPass::Render()
	{
		_mzRenderer.SetRenderTarget();
		_mzRenderer.ClearRenderTarget();

		_resourceManager.GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		_resourceManager.GetDeviceContext()->RSSetState(_resourceManager.GetRasterizerState(ResourceManager::eRasterizerState::SOLID));

		_vertexShader->SetShader();

		_pixelShader->SetShaderResourceView("src", _quadBuffer->GetShaderResourceView());
		_pixelShader->CopyAllBufferData();
		_pixelShader->SetShader();

		_resourceManager.GetDeviceContext()->Draw(4, 0);

		_quadBuffer->FlushShaderResourceViews();
	}
}
