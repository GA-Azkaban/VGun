#include "GBufferPass.h"
#include "ObjectManager.h"
#include "ResourceManager.h"
#include "VertexShader.h"
#include "Camera.h"
#include "StaticMeshObject.h"
#include "SkinningMeshObject.h"
#include "OutlinePass.h"
#include "DeferredBuffers.h"
using namespace DirectX;

namespace RocketCore::Graphics
{
	GBufferPass::GBufferPass(DeferredBuffers* deferredBuffers)
		: _deferredBuffers(deferredBuffers), _deviceContext(ResourceManager::Instance().GetDeviceContext())
	{
		// Create depth stencil states
		D3D11_DEPTH_STENCIL_DESC _stencilEnableDesc;
		ZeroMemory(&_stencilEnableDesc, sizeof(_stencilEnableDesc));

		_stencilEnableDesc.DepthEnable = true;
		_stencilEnableDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		_stencilEnableDesc.DepthFunc = D3D11_COMPARISON_LESS;
		_stencilEnableDesc.StencilEnable = true;
		_stencilEnableDesc.StencilReadMask = 0xFF;
		_stencilEnableDesc.StencilWriteMask = 0xFF;
		_stencilEnableDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		_stencilEnableDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		_stencilEnableDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		_stencilEnableDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		_stencilEnableDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		_stencilEnableDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		_stencilEnableDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		_stencilEnableDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;

		ResourceManager::Instance().GetDevice()->CreateDepthStencilState(&_stencilEnableDesc, _stencilEnableState.GetAddressOf());
	}

	GBufferPass::~GBufferPass()
	{
		delete _deferredBuffers;
		_deviceContext.Reset();
	}

	void GBufferPass::Render()
	{
		_deferredBuffers->SetRenderTargets();
		_deferredBuffers->ClearRenderTargets();

		//XMMATRIX view = Camera::GetMainCamera()->GetViewMatrix();
		//XMMATRIX proj = Camera::GetMainCamera()->GetProjectionMatrix();
		//VertexShader* vertexShader = ResourceManager::Instance().GetVertexShader("VertexShader.cso");
		//vertexShader->SetMatrix4x4("viewProjection", XMMatrixTranspose(view * proj));
		//VertexShader* vertexShader = ResourceManager::Instance().GetVertexShader("SkeletonVertexShader.cso");
		//vertexShader->SetMatrix4x4("viewProjection", XMMatrixTranspose(view * proj));

		for (auto staticMeshObj : ObjectManager::Instance().GetStaticMeshObjList())
		{
			staticMeshObj->Render();
		}

		for (auto skinningMeshObj : ObjectManager::Instance().GetSkinningMeshObjList())
		{
			if(skinningMeshObj->IsOutlineActive())
				continue;
			skinningMeshObj->Render();
		}

		for (UINT i = 0; i < OutlinePass::outlineObjects.size(); ++i)
		{
			_deviceContext->OMSetDepthStencilState(_stencilEnableState.Get(), i + 1);
			OutlinePass::outlineObjects[i]->Render();
		}

		ID3D11ShaderResourceView* nullSRV = nullptr;
		_deviceContext->PSSetShaderResources(0, 1, &nullSRV);
		_deviceContext->PSSetShaderResources(1, 1, &nullSRV);
		_deviceContext->PSSetShaderResources(2, 1, &nullSRV);
	}
}
