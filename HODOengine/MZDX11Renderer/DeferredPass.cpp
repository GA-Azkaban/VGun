#include "DeferredPass.h"
#include "MZDX11Renderer.h"
#include "MZCamera.h"
#include "ResourceManager.h"
#include "RasterizerState.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "DeferredBuffers.h"
#include "QuadBuffer.h"

DeferredPass::DeferredPass(DeferredBuffers* deferredBuffers, QuadBuffer* quadBuffer)
	: _deferredBuffers(deferredBuffers), _quadBuffer(quadBuffer)
{
	_vertexShader = ResourceManager::Instance.Get().GetVertexShader("FullScreenQuadVertexShader.cso");
	_pixelShader = ResourceManager::Instance.Get().GetPixelShader("FullScreenQuadPixelShader.cso");
}

void DeferredPass::Render()
{
	_quadBuffer->SetRenderTargets();
	_quadBuffer->ClearRenderTargets();

	MZDX11Renderer::Instance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	MZDX11Renderer::Instance().GetDeviceContext()->RSSetState(RasterizerState::Instance.Get().GetSolidRS());

	_vertexShader->SetShader();

	XMFLOAT3 cameraPos = MZCamera::GetMainCamera()->GetPosition();
	_pixelShader->SetFloat3("cameraPosition", cameraPos);

	XMMATRIX invView = XMMatrixInverse(0, MZCamera::GetMainCamera()->View());
	_pixelShader->SetMatrix4x4Array("inverseView", &invView);

	XMMATRIX invProj = XMMatrixInverse(0, MZCamera::GetMainCamera()->Proj());
	_pixelShader->SetMatrix4x4Array("inverseProjection", &invProj);

	//_pixelShader->SetShaderResourceView("Position", _deferredBuffers->GetShaderResourceView(0));
	_pixelShader->SetShaderResourceView("DepthTexture", _deferredBuffers->GetDepthSRV());
	_pixelShader->SetShaderResourceView("Diffuse", _deferredBuffers->GetShaderResourceView(0));
	_pixelShader->SetShaderResourceView("Normal", _deferredBuffers->GetShaderResourceView(1));

	_pixelShader->CopyAllBufferData();
	_pixelShader->SetShader();

	MZDX11Renderer::Instance().GetDeviceContext()->Draw(4, 0);

	_deferredBuffers->FlushShaderResourceViews();
}

