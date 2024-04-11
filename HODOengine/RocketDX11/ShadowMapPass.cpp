#include "ShadowMapPass.h"
#include "DeferredBuffers.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ResourceManager.h"
#include "ObjectManager.h"
#include "StaticMeshObject.h"
#include "SkinningMeshObject.h"
#include "Mesh.h"
#include "Camera.h"
#include "Light.h"
#include <vector>
using namespace DirectX;

namespace RocketCore::Graphics
{

	ShadowMapPass::ShadowMapPass(DeferredBuffers* deferredBuffers)
		: _deferredBuffers(deferredBuffers), _deviceContext(ResourceManager::Instance().GetDeviceContext())
	{
		_pixelShader = ResourceManager::Instance().GetPixelShader("ShadowMapPS.cso");
	}

	ShadowMapPass::~ShadowMapPass()
	{
		delete _deferredBuffers;
		_deviceContext.Reset();
	}

	void ShadowMapPass::Render()
	{
		_deferredBuffers->SetShadowMapRenderTarget();
		_deferredBuffers->ClearShadowMapRenderTarget();

		_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		_deviceContext->RSSetState(ResourceManager::Instance().GetRasterizerState(ResourceManager::eRasterizerState::SHADOWMAP));

		_vertexShader = ResourceManager::Instance().GetVertexShader("StaticMeshShadowMapVS.cso");
		
		XMMATRIX lightView = LightManager::Instance().GetLightView();
		XMMATRIX lightProj = LightManager::Instance().GetLightProj();
		XMMATRIX lightViewProj = lightView * lightProj;

		_vertexShader->SetMatrix4x4("lightViewProjection", XMMatrixTranspose(lightViewProj));

		for (auto staticMeshObj : ObjectManager::Instance().GetStaticMeshObjList())
		{
			if(!staticMeshObj->IsActive())
				continue;

			XMMATRIX world = staticMeshObj->GetWorldTM();

			_vertexShader->SetMatrix4x4("world", XMMatrixTranspose(world));

			_vertexShader->CopyAllBufferData();
			_vertexShader->SetShader();

			_pixelShader->SetShader();

			for (auto mesh : staticMeshObj->GetMeshes())
			{
				mesh->BindBuffers();
				mesh->Draw();
			}
		}

		_vertexShader = ResourceManager::Instance().GetVertexShader("SkinningMeshShadowMapVS.cso");
		_vertexShader->SetMatrix4x4("lightViewProjection", XMMatrixTranspose(lightViewProj));

		for (auto skinningMeshObj : ObjectManager::Instance().GetSkinningMeshObjList())
		{
			if (!skinningMeshObj->IsActive())
				continue;

			XMMATRIX world = skinningMeshObj->GetWorldTM();

			_vertexShader->SetMatrix4x4("world", XMMatrixTranspose(world));
			_vertexShader->SetMatrix4x4Array("boneTransforms", &skinningMeshObj->GetBoneTransforms()[0], skinningMeshObj->GetBoneTransforms().size());

			_vertexShader->CopyAllBufferData();
			_vertexShader->SetShader();

			_pixelShader->SetShader();

			for (auto mesh : skinningMeshObj->GetMeshes())
			{
				mesh->BindBuffers();
				mesh->Draw();
			}
		}

		_deferredBuffers->ClearDepthStencil();
	}

}
