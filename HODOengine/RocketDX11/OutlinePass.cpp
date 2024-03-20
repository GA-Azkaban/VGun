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

	OutlinePass::OutlinePass(DeferredBuffers* deferredBuffers, QuadBuffer* quadBuffer, QuadBuffer* stencilEnableBuffer)
		: _deferredBuffers(deferredBuffers), _quadBuffer(quadBuffer), _stencilEnableBuffer(stencilEnableBuffer),
		_deviceContext(ResourceManager::Instance().GetDeviceContext())
	{
		// Create depth stencil states
		D3D11_DEPTH_STENCIL_DESC depthEnableDesc;
		ZeroMemory(&depthEnableDesc, sizeof(depthEnableDesc));

		depthEnableDesc.DepthEnable = true;
		depthEnableDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		depthEnableDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthEnableDesc.StencilEnable = true;
		depthEnableDesc.StencilReadMask = 0xFF;
		depthEnableDesc.StencilWriteMask = 0xFF;
		depthEnableDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthEnableDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		depthEnableDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		depthEnableDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depthEnableDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthEnableDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		depthEnableDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthEnableDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;

		ResourceManager::Instance().GetDevice()->CreateDepthStencilState(&depthEnableDesc, _depthEnableState.GetAddressOf());

		D3D11_DEPTH_STENCIL_DESC depthDisableDesc;
		ZeroMemory(&depthDisableDesc, sizeof(depthDisableDesc));

		depthDisableDesc.DepthEnable = false;
		depthDisableDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		depthDisableDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
		depthDisableDesc.StencilEnable = true;
		depthDisableDesc.StencilReadMask = 0xFF;
		depthDisableDesc.StencilWriteMask = 0xFF;
		depthDisableDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthDisableDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		depthDisableDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthDisableDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
		depthDisableDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthDisableDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		depthDisableDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthDisableDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;

		ResourceManager::Instance().GetDevice()->CreateDepthStencilState(&depthDisableDesc, _depthDisableState.GetAddressOf());
	}

	OutlinePass::~OutlinePass()
	{
		_deferredBuffers = nullptr;
		_quadBuffer = nullptr;
		_stencilEnableBuffer = nullptr;
		delete _deferredBuffers;
		delete _quadBuffer;
		delete _stencilEnableBuffer;

		_deviceContext.Reset();
	}

	void OutlinePass::Render()
	{
		// 외곽선을 만드는 것은 두 단계로 나눠서 한다.
		// 첫번째 단계는 스텐실버퍼에 1 이상의 값으로 기록된 부분에
		// 색깔을 입혀 렌더타겟뷰에 그린다.
		// 두번째 단계는 렌더타겟뷰를 셰이더리소스뷰로 가져와서
		// 소벨 가장자리 검출 단계를 거쳐 외곽선부분만 딴다.
		// 외곽선부분을 딴 곳에 외곽선 색깔을 입혀 최종적으로 출력한다.
		
		// 첫번째 단계
		//_quadBuffer->SetRenderTargets(_deferredBuffers->GetDepthStencilView());
		_stencilEnableBuffer->SetRenderTargets(_deferredBuffers->GetDepthStencilView());
		_stencilEnableBuffer->ClearRenderTargets();

		_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		_deviceContext->RSSetState(ResourceManager::Instance().GetRasterizerState(ResourceManager::eRasterizerState::SOLID));
		
		_deviceContext->OMSetDepthStencilState(_depthDisableState.Get(), 1);

		//_vertexShader = ResourceManager::Instance().GetVertexShader("FullScreenQuadVS_3Vertex.cso");
		_vertexShader = ResourceManager::Instance().GetVertexShader("FullScreenQuadVS.cso");
		_pixelShader = ResourceManager::Instance().GetPixelShader("Outline_StencilColorPS.cso");

		_vertexShader->SetShader();

		_pixelShader->SetFloat4("outlineColor", XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
		_pixelShader->CopyAllBufferData();
		_pixelShader->SetShader();

		_deviceContext->Draw(4, 0);

		_deviceContext->OMSetDepthStencilState(_depthDisableState.Get(), 2);

		//_vertexShader->SetShader();

		_pixelShader->SetFloat4("outlineColor", XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
		_pixelShader->CopyAllBufferData();
		_pixelShader->SetShader();

		_deviceContext->Draw(4, 0);

		// 두번째 단계
		_quadBuffer->SetRenderTargets();

		_pixelShader = ResourceManager::Instance().GetPixelShader("Outline_SobelDetectionPS.cso");

		_pixelShader->SetFloat4("outlineColor", XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
		_pixelShader->SetFloat("outlineThreshHold", 0.5f);
		_pixelShader->SetFloat("outlineThickness", 2.0f);
		_pixelShader->SetShaderResourceView("StencilColorTex", _stencilEnableBuffer->GetShaderResourceView());
		_pixelShader->CopyAllBufferData();
		_pixelShader->SetShader();

		_deviceContext->Draw(4, 0);

		_quadBuffer->FlushShaderResourceViews();
	}

}
