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

		_proj = XMMatrixOrthographicLH(16, 12, 1, 20);
		_directionalLight = &(LightManager::Instance().GetLightProperties().lights[0]);
	}

	ShadowMapPass::~ShadowMapPass()
	{
		delete _deferredBuffers;
	}

	void ShadowMapPass::Render()
	{
		_deferredBuffers->SetShadowMapRenderTarget();
		_deferredBuffers->ClearShadowMapRenderTarget();

		_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		_deviceContext->RSSetState(ResourceManager::Instance().GetRasterizerState(ResourceManager::eRasterizerState::SOLID));

		_vertexShader = ResourceManager::Instance().GetVertexShader("VertexShader.cso");
		XMVECTOR lightDir = XMLoadFloat4(&_directionalLight->direction);
		//XMVECTOR lightDirNorm = XMVector4Normalize(lightDir);
		//_view = XMMatrixLookAtLH(XMVECTOR{10,10,0,0}, XMVECTOR{0, 0, 0, 1}, XMVECTOR{0, 1, 0, 0});
		_view = XMMatrixLookAtLH(-10 * lightDir, XMVECTOR{0, 0, 0, 1}, XMVECTOR{0, 1, 0, 0});

		for (auto staticMeshObj : ObjectManager::Instance().GetStaticMeshObjList())
		{
			if(!staticMeshObj->IsActive())
				continue;

			XMMATRIX world = staticMeshObj->GetWorldTM();
			XMMATRIX worldViewProj = world * _view * _proj;

			_vertexShader->SetMatrix4x4("world", XMMatrixTranspose(world));
			_vertexShader->SetMatrix4x4("worldViewProj", XMMatrixTranspose(worldViewProj));

			_vertexShader->CopyAllBufferData();
			_vertexShader->SetShader();

			_pixelShader->SetShader();

			for (auto mesh : staticMeshObj->GetMeshes())
			{
				mesh->BindBuffers();
				mesh->Draw();
			}
		}

		_vertexShader = ResourceManager::Instance().GetVertexShader("SkeletonVertexShader.cso");
		for (auto skinningMeshObj : ObjectManager::Instance().GetSkinningMeshObjList())
		{
			if (!skinningMeshObj->IsActive())
				continue;

			XMMATRIX world = skinningMeshObj->GetWorldTM();
			XMMATRIX worldViewProj = world * _view * _proj;

			_vertexShader->SetMatrix4x4("world", XMMatrixTranspose(world));
			_vertexShader->SetMatrix4x4("worldViewProj", XMMatrixTranspose(worldViewProj));
			_vertexShader->SetMatrix4x4Array("boneTransforms", &skinningMeshObj->GetBoneTransform()[0], skinningMeshObj->GetBoneTransform().size());

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
