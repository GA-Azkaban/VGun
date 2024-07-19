#include "ParticlePass.h"
#include "DeferredBuffers.h"
#include "QuadBuffer.h"
#include "ResourceManager.h"
#include "ObjectManager.h"
#include "ParticleSystem.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Camera.h"
#include <vector>
#include <DirectXMath.h>
using namespace DirectX;

namespace RocketCore::Graphics
{

	ParticlePass::ParticlePass(DeferredBuffers* deferredBuffers, QuadBuffer* quadBuffer)
		: _deferredBuffers(deferredBuffers), _quadBuffer(quadBuffer)
	{
		D3D11_BLEND_DESC blendDesc;
		blendDesc.AlphaToCoverageEnable = false;
		blendDesc.IndependentBlendEnable = false;
		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		ResourceManager::Instance().GetDevice()->CreateBlendState(&blendDesc, _additiveBlendState.GetAddressOf());

		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;

		ResourceManager::Instance().GetDevice()->CreateBlendState(&blendDesc, _defaultBlendState.GetAddressOf());


	}

	ParticlePass::~ParticlePass()
	{
		delete _deferredBuffers;
		delete _quadBuffer;
	}

	void ParticlePass::Render()
	{
		_quadBuffer->SetRenderTargets(_deferredBuffers->GetDepthStencilView());

		XMMATRIX view = Camera::GetMainCamera()->GetViewMatrix();
		XMMATRIX proj = Camera::GetMainCamera()->GetProjectionMatrix();
		VertexShader* vertexShader = ResourceManager::Instance().GetVertexShader("ParticleVertexShader.cso");
		vertexShader->SetMatrix4x4("viewProjection", XMMatrixTranspose(view * proj));

		std::vector<ParticleSystem*>& psList = ObjectManager::Instance().GetParticleSystemList();
		for (int i = 0; i < psList.size(); ++i)
		{
			if (psList[i]->IsAlphaBlend())
			{
				ResourceManager::Instance().GetDeviceContext()->OMSetBlendState(_additiveBlendState.Get(), _blendFactor, 0xFFFFFFFF);
			}
			else
			{
				ResourceManager::Instance().GetDeviceContext()->OMSetBlendState(_defaultBlendState.Get(), _blendFactor, 0xFFFFFFFF);
			}
			psList[i]->Render();
		}

		ID3D11ShaderResourceView* nullSRV = nullptr;
		ResourceManager::Instance().GetDeviceContext()->PSSetShaderResources(0, 1, &nullSRV);
		ID3D11BlendState* nullBlend = nullptr;
		ResourceManager::Instance().GetDeviceContext()->OMSetBlendState(nullBlend, _blendFactor, 0xFFFFFFFF);
	}

}
