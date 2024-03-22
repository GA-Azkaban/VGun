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
#include "Camera.h"
using namespace DirectX;

namespace RocketCore::Graphics
{
	std::vector<SkinningMeshObject*> OutlinePass::outlineObjects;

	OutlinePass::OutlinePass(DeferredBuffers* deferredBuffers, QuadBuffer* quadBuffer, QuadBuffer* stencilEnableBuffer)
		: _deferredBuffers(deferredBuffers), _quadBuffer(quadBuffer), _stencilEnableBuffer(stencilEnableBuffer),
		_deviceContext(ResourceManager::Instance().GetDeviceContext())
	{
		// Create depth stencil states
		D3D11_DEPTH_STENCIL_DESC stencilColorDesc;
		ZeroMemory(&stencilColorDesc, sizeof(stencilColorDesc));

		stencilColorDesc.DepthEnable = false;
		stencilColorDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		stencilColorDesc.DepthFunc = D3D11_COMPARISON_NEVER;
		stencilColorDesc.StencilEnable = true;
		stencilColorDesc.StencilReadMask = 0xFF;
		stencilColorDesc.StencilWriteMask = 0xFF;
		stencilColorDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		stencilColorDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		stencilColorDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		stencilColorDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
		stencilColorDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		stencilColorDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		stencilColorDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		stencilColorDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;

		ResourceManager::Instance().GetDevice()->CreateDepthStencilState(&stencilColorDesc, _stencilColorState.GetAddressOf());

		D3D11_DEPTH_STENCIL_DESC sobelDetectionDesc;
		ZeroMemory(&sobelDetectionDesc, sizeof(sobelDetectionDesc));

		sobelDetectionDesc.DepthEnable = false;
		sobelDetectionDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		sobelDetectionDesc.DepthFunc = D3D11_COMPARISON_NEVER;
		sobelDetectionDesc.StencilEnable = true;
		sobelDetectionDesc.StencilReadMask = 0xFF;
		sobelDetectionDesc.StencilWriteMask = 0xFF;
		sobelDetectionDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		sobelDetectionDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		sobelDetectionDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		sobelDetectionDesc.FrontFace.StencilFunc = D3D11_COMPARISON_LESS_EQUAL;
		sobelDetectionDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		sobelDetectionDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		sobelDetectionDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		sobelDetectionDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;

		ResourceManager::Instance().GetDevice()->CreateDepthStencilState(&sobelDetectionDesc, _sobelDetectionState.GetAddressOf());

		D3D11_DEPTH_STENCIL_DESC stencilDisableDesc;
		ZeroMemory(&stencilDisableDesc, sizeof(stencilDisableDesc));

		stencilDisableDesc.DepthEnable = true;
		stencilDisableDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		stencilDisableDesc.DepthFunc = D3D11_COMPARISON_LESS;
		stencilDisableDesc.StencilEnable = false;

		ResourceManager::Instance().GetDevice()->CreateDepthStencilState(&stencilDisableDesc, _stencilDisableState.GetAddressOf());

		// Create blend states
		D3D11_BLEND_DESC outlineBlendDesc;
		outlineBlendDesc.AlphaToCoverageEnable = false;
		outlineBlendDesc.IndependentBlendEnable = false;
		outlineBlendDesc.RenderTarget[0].BlendEnable = true;
		outlineBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		outlineBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		outlineBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		outlineBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		outlineBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		outlineBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		outlineBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		ResourceManager::Instance().GetDevice()->CreateBlendState(&outlineBlendDesc, _outlineBlendState.GetAddressOf());

		stencilColors[0] = XMFLOAT4{ 1.0f, 0.0f, 0.0f, 1.0f };
		stencilColors[1] = XMFLOAT4{ 0.0f, 1.0f, 0.0f, 1.0f };
		stencilColors[2] = XMFLOAT4{ 0.0f, 0.0f, 1.0f, 1.0f };
		stencilColors[3] = XMFLOAT4{ 1.0f, 1.0f, 0.0f, 1.0f };
		stencilColors[4] = XMFLOAT4{ 1.0f, 0.0f, 1.0f, 1.0f };
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

		_deviceContext->RSSetState(ResourceManager::Instance().GetRasterizerState(ResourceManager::eRasterizerState::SOLID));
		_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		// 첫번째 단계
		_stencilEnableBuffer->SetRenderTargets(_deferredBuffers->GetDepthStencilView());
		_stencilEnableBuffer->ClearRenderTargets(XMVECTOR{ 0.0f, 0.0f, 0.0f, 1.0f });

		_vertexShader = ResourceManager::Instance().GetVertexShader("FullScreenQuadVS.cso");
		_vertexShader->SetShader();
		_pixelShader = ResourceManager::Instance().GetPixelShader("Outline_StencilColorPS.cso");

		for (UINT i = 0; i < outlineObjects.size(); ++i)
		{
			_deviceContext->OMSetDepthStencilState(_stencilColorState.Get(), i + 1);

			_pixelShader->SetFloat4("outlineColor", stencilColors[i]);
			_pixelShader->CopyAllBufferData();
			_pixelShader->SetShader();

			_deviceContext->Draw(4, 0);
		}

		// 두번째 단계
		_quadBuffer->SetRenderTargets(_deferredBuffers->GetDepthStencilView());

		_deviceContext->OMSetDepthStencilState(_sobelDetectionState.Get(), 1);
		_deviceContext->OMSetBlendState(_outlineBlendState.Get(), nullptr, 0xffffffff);

		_pixelShader = ResourceManager::Instance().GetPixelShader("Outline_SobelDetectionPS.cso");

		_pixelShader->SetFloat4("outlineColor", XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
		_pixelShader->SetFloat("outlineThreshHold", 0.5f);
		_pixelShader->SetFloat("outlineThickness", 1.25f);
		_pixelShader->SetFloat("screenWidth", static_cast<float>(_deferredBuffers->GetScreenWidth()));
		_pixelShader->SetFloat("screenHeight", static_cast<float>(_deferredBuffers->GetScreenHeight()));
		_pixelShader->SetShaderResourceView("StencilColorTex", _stencilEnableBuffer->GetShaderResourceView());
		_pixelShader->CopyAllBufferData();
		_pixelShader->SetShader();

		_deviceContext->Draw(4, 0);

		_quadBuffer->FlushShaderResourceViews();

		_deviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
		_deviceContext->OMSetDepthStencilState(_stencilDisableState.Get(), 0);
	}

}
