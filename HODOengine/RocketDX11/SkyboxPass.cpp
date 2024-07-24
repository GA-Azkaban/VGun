#include "SkyboxPass.h"
#include "DeferredBuffers.h"
#include "QuadBuffer.h"
#include "ObjectManager.h"
#include "Cubemap.h"
#include "ResourceManager.h"

namespace RocketCore::Graphics
{
	SkyboxPass::SkyboxPass(DeferredBuffers* deferredBuffers, QuadBuffer* quadBuffer)
		: _deferredBuffers(deferredBuffers), _quadBuffer(quadBuffer)
	{

	}

	SkyboxPass::~SkyboxPass()
	{
		delete _deferredBuffers;
		delete _quadBuffer;
	}

	void SkyboxPass::Render()
	{
		_quadBuffer->SetRenderTargets(_deferredBuffers->GetDepthStencilView());

		Cubemap::Instance().Render();

		ID3D11ShaderResourceView* nullSRV = nullptr;
		ResourceManager::Instance().GetDeviceContext()->PSSetShaderResources(0, 1, &nullSRV);
	}
}
