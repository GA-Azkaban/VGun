#include "OutlinePass.h"
#include "DeferredBuffers.h"
#include "QuadBuffer.h"
#include "ResourceManager.h"
#include "ObjectManager.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Mesh.h"
#include "StaticMeshObject.h"
#include "SkinningMeshObject.h"
#include "Outline.h"
#include "Camera.h"
using namespace DirectX;

namespace RocketCore::Graphics
{
	std::vector<StaticMeshObject*> OutlinePass::staticMeshOutlines;
	std::vector<SkinningMeshObject*> OutlinePass::skinningMeshOutlines;

	OutlinePass::OutlinePass(DeferredBuffers* deferredBuffers, QuadBuffer* quadBuffer)
		: _deferredBuffers(deferredBuffers), _quadBuffer(quadBuffer),
		_deviceContext(ResourceManager::Instance().GetDeviceContext())
	{
		//_lineScale = XMMatrixScaling(1.05f, 1.01f, 1.05f);
		_lineScale = XMMatrixScaling(0.95f, 0.95f, 0.95f);

		_pixelShader = ResourceManager::Instance().GetPixelShader("OutlinePS.cso");

		// Create depth stencil states
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

		ResourceManager::Instance().GetDevice()->CreateDepthStencilState(&depthStencilDesc, _depthEnableState.GetAddressOf());
	}

	OutlinePass::~OutlinePass()
	{
		_deferredBuffers = nullptr;
		_quadBuffer = nullptr;
	}

	void OutlinePass::Render()
	{
		_quadBuffer->SetRenderTargets(_deferredBuffers->GetDepthStencilView());

		_deviceContext->OMSetDepthStencilState(_depthEnableState.Get(), 0);

		_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		_deviceContext->RSSetState(ResourceManager::Instance().GetRasterizerState(ResourceManager::eRasterizerState::SOLID));

		XMMATRIX view = Camera::GetMainCamera()->GetViewMatrix();
		XMMATRIX proj = Camera::GetMainCamera()->GetProjectionMatrix(); 

		_vertexShader = ResourceManager::Instance().GetVertexShader("OutlineStaticMeshVS.cso");
		for (auto staticMeshObj : staticMeshOutlines)
		{
			if (!staticMeshObj->IsActive())
				continue;

			if (!staticMeshObj->GetOutline()->isActive)
				continue;

			XMMATRIX world = staticMeshObj->GetWorldTM();
			XMVECTOR scale;
			XMVECTOR rotate;
			XMVECTOR translate;
			XMMatrixDecompose(&scale, &rotate, &translate, world);
			XMMATRIX scaleMatrix = XMMatrixScalingFromVector(scale);
			XMMATRIX rotateMatrix = XMMatrixRotationQuaternion(rotate);
			XMMATRIX translateMatrix = XMMatrixTranslationFromVector(translate);
			XMMATRIX newScale = scaleMatrix * _lineScale;
			XMMATRIX worldViewProj = newScale * rotateMatrix * translateMatrix * view * proj;

			_vertexShader->SetMatrix4x4("worldViewProj", XMMatrixTranspose(worldViewProj));
			_vertexShader->SetFloat4("color", staticMeshObj->GetOutline()->color);

			_vertexShader->CopyAllBufferData();
			_vertexShader->SetShader();

			_pixelShader->SetShader();

			for (auto mesh : staticMeshObj->GetMeshes())
			{
				mesh->BindBuffers();
				mesh->Draw();
			}
		}

		_vertexShader = ResourceManager::Instance().GetVertexShader("OutlineSkinningMeshVS.cso");
		for (auto skinningMeshObj : skinningMeshOutlines)
		{
			if (!skinningMeshObj->IsActive())
				continue;

			if (!skinningMeshObj->GetOutline()->isActive)
				continue;
			
			XMMATRIX world = skinningMeshObj->GetWorldTM();
			XMVECTOR scale;
			XMVECTOR rotate;
			XMVECTOR translate;
			XMMatrixDecompose(&scale, &rotate, &translate, world);
			XMMATRIX scaleMatrix = XMMatrixScalingFromVector(scale);
			XMMATRIX rotateMatrix = XMMatrixRotationQuaternion(rotate);
			XMMATRIX translateMatrix = XMMatrixTranslationFromVector(translate);
			XMMATRIX newScale = scaleMatrix * _lineScale;
			XMMATRIX worldViewProj = newScale * rotateMatrix * translateMatrix * view * proj;

			_vertexShader->SetMatrix4x4("worldViewProj", XMMatrixTranspose(worldViewProj));
			_vertexShader->SetFloat4("color", skinningMeshObj->GetOutline()->color);
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
	}

}
