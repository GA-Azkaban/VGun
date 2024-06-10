#include "DecalPass.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "ObjectManager.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "QuadBuffer.h"
#include "DeferredBuffers.h"
#include "DecalObject.h"

namespace RocketCore::Graphics
{

	DecalPass::DecalPass(DeferredBuffers* deferredBuffers, QuadBuffer* quadBuffer)
		: _deferredBuffers(deferredBuffers), _quadBuffer(quadBuffer)
	{

	}

	DecalPass::~DecalPass()
	{
		_deferredBuffers = nullptr;
		_quadBuffer = nullptr;
		_blendState.Reset();
	}

	void DecalPass::Render()
	{
		//_quadBuffer->SetRenderTargets(_deferredBuffers->GetDepthStencilView());
		_quadBuffer->SetRenderTargets();

		ResourceManager::Instance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		ResourceManager::Instance().GetDeviceContext()->RSSetState(ResourceManager::Instance().GetRasterizerState(ResourceManager::eRasterizerState::SOLID));

		XMMATRIX view = Camera::GetMainCamera()->GetViewMatrix();
		XMMATRIX proj = Camera::GetMainCamera()->GetProjectionMatrix();
		VertexShader* vertexShader = ResourceManager::Instance().GetVertexShader("DecalVertexShader.cso");
		vertexShader->SetMatrix4x4("viewProjection", XMMatrixTranspose(view * proj));
		PixelShader* pixelShader = ResourceManager::Instance().GetPixelShader("DecalPixelShader.cso");
		XMMATRIX invView = XMMatrixInverse(nullptr, view);
		XMMATRIX invProj = XMMatrixInverse(nullptr, proj);
		pixelShader->SetMatrix4x4("inverseView", (invView));
		pixelShader->SetMatrix4x4("inverseProjection", (invProj));
		pixelShader->SetShaderResourceView("DepthTex", _deferredBuffers->GetDepthSRV());

		for (auto decal : ObjectManager::Instance().GetDecalObjList())
		{
			decal->Render();
		}

		ID3D11ShaderResourceView* nullSRV = nullptr;
		ResourceManager::Instance().GetDeviceContext()->PSSetShaderResources(0, 1, &nullSRV);
		ResourceManager::Instance().GetDeviceContext()->PSSetShaderResources(1, 1, &nullSRV);
	}

}
