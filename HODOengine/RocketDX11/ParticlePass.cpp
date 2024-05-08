#include "ParticlePass.h"
#include "QuadBuffer.h"
#include "BillboardParticle.h"
#include "ResourceManager.h"

namespace RocketCore::Graphics
{

	ParticlePass::ParticlePass(QuadBuffer* toneMapBuffer)
		: _toneMapBuffer(toneMapBuffer)
	{
		_billboard = new BillboardParticle();

		D3D11_BLEND_DESC blendDesc;
		blendDesc.AlphaToCoverageEnable = false;
		blendDesc.IndependentBlendEnable = false;
		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		//blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		//blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		//blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		//blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		ResourceManager::Instance().GetDevice()->CreateBlendState(&blendDesc, _blendState.GetAddressOf());
	}

	ParticlePass::~ParticlePass()
	{
		_toneMapBuffer = nullptr;
	}

	void ParticlePass::Render()
	{
		_toneMapBuffer->SetRenderTargets();
		ResourceManager::Instance().GetDeviceContext()->OMSetBlendState(_blendState.Get(), _blendFactor, 0xFFFFFFFF);
		_billboard->Render();

		ID3D11ShaderResourceView* nullSRV = nullptr;
		ResourceManager::Instance().GetDeviceContext()->PSSetShaderResources(0, 1, &nullSRV);
		ID3D11BlendState* nullBlend = nullptr;
		ResourceManager::Instance().GetDeviceContext()->OMSetBlendState(nullBlend, _blendFactor, 0xFFFFFFFF);
	}

}
