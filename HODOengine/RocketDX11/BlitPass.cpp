#include "BlitPass.h"
#include "ResourceManager.h"
#include "QuadBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include <DirectXMath.h>
#include <DirectXColors.h>

namespace RocketCore::Graphics
{
	BlitPass::BlitPass(QuadBuffer* quadBuffer, ID3D11RenderTargetView* backBufferRTV)
		: _quadBuffer(quadBuffer), _resourceManager(ResourceManager::Instance()),
		_backBufferRTV(backBufferRTV)
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
		_resourceManager.GetDeviceContext()->OMSetRenderTargets(1, _backBufferRTV.GetAddressOf(), nullptr);
		_resourceManager.GetDeviceContext()->ClearRenderTargetView(_backBufferRTV.Get(), reinterpret_cast<const float*>(&DirectX::Colors::Transparent));

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
