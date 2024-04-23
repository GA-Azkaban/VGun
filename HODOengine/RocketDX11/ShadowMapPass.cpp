#include "ShadowMapPass.h"
#include "DeferredBuffers.h"
#include "VertexShader.h"
#include "GeometryShader.h"
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

	ShadowMapPass::ShadowMapPass()
		: _deviceContext(ResourceManager::Instance().GetDeviceContext())
	{
		// Create shadow map texture, RTV, SRV
		D3D11_TEXTURE2D_DESC shadowMapTexDesc{
			.Width = SHADOW_MAP_SIZE,
			.Height = SHADOW_MAP_SIZE,
			.MipLevels = 1,
			.ArraySize = CASCADE_COUNT,
			.Format = DXGI_FORMAT_R32_TYPELESS,
			.SampleDesc{.Count = 1 },
			.Usage = D3D11_USAGE_DEFAULT,
			.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE,
			.CPUAccessFlags = 0,
			.MiscFlags = 0
		};

		ResourceManager::Instance().GetDevice()->CreateTexture2D(&shadowMapTexDesc, nullptr, _shadowMapTexture.GetAddressOf());

		for (UINT i = 0; i < CASCADE_COUNT; ++i)
		{
			D3D11_DEPTH_STENCIL_VIEW_DESC shadowMapDSVDesc;
			shadowMapDSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
			shadowMapDSVDesc.Texture2DArray.FirstArraySlice = i;
			shadowMapDSVDesc.Texture2DArray.ArraySize = 1;
			shadowMapDSVDesc.Texture2DArray.MipSlice = 0;

			ResourceManager::Instance().GetDevice()->CreateDepthStencilView(_shadowMapTexture.Get(), &shadowMapDSVDesc, _shadowMapDSV[i].GetAddressOf());
		}

		//D3D11_SHADER_RESOURCE_VIEW_DESC shadowMapSRVDesc;
		//shadowMapSRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
		//shadowMapSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		//shadowMapSRVDesc.Texture2D.MostDetailedMip = 0;
		//shadowMapSRVDesc.Texture2D.MipLevels = 1;
		//
		//_device->CreateShaderResourceView(_shadowMapTexture.Get(), &shadowMapSRVDesc, _shadowMapShaderResourceView.GetAddressOf());

		_geometryShader =
			_pixelShader = ResourceManager::Instance().GetPixelShader("ShadowMapPS.cso");
	}

	ShadowMapPass::~ShadowMapPass()
	{
		_deviceContext.Reset();
	}

	void ShadowMapPass::Render()
	{
		_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		_deviceContext->RSSetState(ResourceManager::Instance().GetRasterizerState(ResourceManager::eRasterizerState::SHADOWMAP));
		// 추후에 프러스텀에 들어온 오브젝트만 그리게 해야한다.
		for (UINT i = 0; i < CASCADE_COUNT; ++i)
		{
			_deviceContext->OMSetRenderTargets(1, _shadowMapRenderTargetView.GetAddressOf(), _shadowMapDSV.Get());
			_deviceContext->ClearRenderTargetView(_shadowMapRenderTargetView.Get(), reinterpret_cast<const float*>(&color));
			_deviceContext->ClearDepthStencilView(_shadowMapDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

			_vertexShader = ResourceManager::Instance().GetVertexShader("StaticMeshShadowMapVS.cso");

			XMMATRIX lightView = LightManager::Instance().GetLightView();
			XMMATRIX lightProj = LightManager::Instance().GetLightProj();
			XMMATRIX lightViewProj = lightView * lightProj;

			_vertexShader->SetMatrix4x4("lightViewProjection", XMMatrixTranspose(lightViewProj));

			for (auto staticMeshObj : ObjectManager::Instance().GetStaticMeshObjList())
			{
				if (!staticMeshObj->IsActive())
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
		}
	}

}
