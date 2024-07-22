#include "ForwardPass.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "VertexShader.h"
#include "QuadBuffer.h"
#include "DeferredBuffers.h"
#include "ObjectManager.h"
#include "StaticMeshObject.h"

using namespace DirectX;

namespace RocketCore::Graphics
{
	ForwardPass::ForwardPass(DeferredBuffers* deferredBuffers, QuadBuffer* quadBuffer)
		: _deferredBuffers(deferredBuffers), _quadBuffer(quadBuffer)
	{
		
	}

	ForwardPass::~ForwardPass()
	{
		delete _quadBuffer;
	}

	void ForwardPass::Render()
	{
		_quadBuffer->SetRenderTargets(_deferredBuffers->GetDepthStencilView());
		//_quadBuffer->ClearRenderTargets();
		//_deferredBuffers->ClearDepthStencil();

		ResourceManager::Instance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		ResourceManager::Instance().GetDeviceContext()->RSSetState(ResourceManager::Instance().GetRasterizerState(ResourceManager::eRasterizerState::SOLID));

		XMMATRIX view = Camera::GetMainCamera()->GetViewMatrix();
		XMMATRIX proj = Camera::GetMainCamera()->GetProjectionMatrix();
		VertexShader* vertexShader = ResourceManager::Instance().GetVertexShader("VertexShader.cso");
		vertexShader->SetMatrix4x4("viewProjection", XMMatrixTranspose(view * proj));
		vertexShader = ResourceManager::Instance().GetVertexShader("SkeletonVertexShader.cso");
		vertexShader->SetMatrix4x4("viewProjection", XMMatrixTranspose(view * proj));

		for (auto staticMeshObj : ObjectManager::Instance().GetFowardStaticMeshObjList())
		{
			staticMeshObj->Render();
		}

		ID3D11ShaderResourceView* nullSRV = nullptr;
		ResourceManager::Instance().GetDeviceContext()->PSSetShaderResources(0, 1, &nullSRV);
		ResourceManager::Instance().GetDeviceContext()->PSSetShaderResources(1, 1, &nullSRV);
		ResourceManager::Instance().GetDeviceContext()->PSSetShaderResources(2, 1, &nullSRV);
		ResourceManager::Instance().GetDeviceContext()->PSSetShaderResources(3, 1, &nullSRV);
		ResourceManager::Instance().GetDeviceContext()->PSSetShaderResources(4, 1, &nullSRV);
		ResourceManager::Instance().GetDeviceContext()->PSSetShaderResources(5, 1, &nullSRV);
	}

}
