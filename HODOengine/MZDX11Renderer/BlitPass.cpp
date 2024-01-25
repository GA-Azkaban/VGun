#include "BlitPass.h"
#include "MZDX11Renderer.h"
#include "ResourceManager.h"
#include "QuadBuffer.h"
#include "RasterizerState.h"
#include "VertexShader.h"
#include "PixelShader.h"

BlitPass::BlitPass(QuadBuffer* quadBuffer)
	: _quadBuffer(quadBuffer), _mzRenderer(MZDX11Renderer::Instance())
{
	_vertexShader = ResourceManager::Instance.Get().GetVertexShader("FullScreenQuadVertexShader.cso");
	_pixelShader = ResourceManager::Instance.Get().GetPixelShader("BlitPixelShader.cso");
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

	_mzRenderer.GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	_mzRenderer.GetDeviceContext()->RSSetState(RasterizerState::Instance.Get().GetSolidRS());

	_vertexShader->SetShader();

	_pixelShader->SetShaderResourceView("src", _quadBuffer->GetShaderResourceView());
	_pixelShader->CopyAllBufferData();
	_pixelShader->SetShader();

	_mzRenderer.GetDeviceContext()->Draw(4, 0);

	_quadBuffer->FlushShaderResourceViews();
}
