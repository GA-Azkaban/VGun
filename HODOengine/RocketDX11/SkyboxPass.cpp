#include "SkyboxPass.h"
#include "DeferredBuffers.h"
#include "QuadBuffer.h"
#include "ObjectManager.h"
#include "Cubemap.h"

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

		Cubemap::Instance()->Render();
	}
}
