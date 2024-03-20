#include "GBufferPass.h"
#include "ObjectManager.h"
#include "ResourceManager.h"
#include "VertexShader.h"
#include "Camera.h"
#include "StaticMeshObject.h"
#include "SkinningMeshObject.h"
#include "DeferredBuffers.h"
using namespace DirectX;

namespace RocketCore::Graphics
{
	GBufferPass::GBufferPass(DeferredBuffers* deferredBuffers)
		: _deferredBuffers(deferredBuffers), _deviceContext(ResourceManager::Instance().GetDeviceContext())
	{

	}

	GBufferPass::~GBufferPass()
	{
		delete _deferredBuffers;
	}

	void GBufferPass::Render()
	{
		_deferredBuffers->SetRenderTargets();
		_deferredBuffers->ClearRenderTargets();

		//XMFLOAT3 cameraPos = Camera::GetMainCamera()->GetPosition();
		XMMATRIX view = Camera::GetMainCamera()->GetViewMatrix();
		XMMATRIX proj = Camera::GetMainCamera()->GetProjectionMatrix();
		VertexShader* vertexShader = ResourceManager::Instance().GetVertexShader("VertexShader.cso");
		vertexShader->SetMatrix4x4("viewProjection", XMMatrixTranspose(view * proj));
		vertexShader = ResourceManager::Instance().GetVertexShader("SkeletonVertexShader.cso");
		vertexShader->SetMatrix4x4("viewProjection", XMMatrixTranspose(view * proj));
		//vertexShader->SetMatrix4x4("view", XMMatrixTranspose(view));
		//vertexShader->SetMatrix4x4("projection", XMMatrixTranspose(proj));
		//vertexShader->SetMatrix4x4("inverseProjection", XMMatrixInverse(nullptr, proj));
		//vertexShader->SetFloat4("cameraPosition", XMFLOAT4(cameraPos.x, cameraPos.y, cameraPos.z, 1.0f));
		//vertexShader->SetFloat("screenWidth", _deferredBuffers->GetScreenWidth());
		//vertexShader->SetFloat("screenHeight", _deferredBuffers->GetScreenHeight());

		for (auto staticMeshObj : ObjectManager::Instance().GetStaticMeshObjList())
		{
			staticMeshObj->Render();
		}

		for (auto skinningMeshObj : ObjectManager::Instance().GetSkinningMeshObjList())
		{
			skinningMeshObj->Render();
		}

		ID3D11ShaderResourceView* nullSRV = nullptr;
		_deviceContext->PSSetShaderResources(0, 1, &nullSRV);
		_deviceContext->PSSetShaderResources(1, 1, &nullSRV);
		_deviceContext->PSSetShaderResources(2, 1, &nullSRV);
	}
}
