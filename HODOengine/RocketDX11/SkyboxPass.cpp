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

	void SkyboxPass::Render()
	{
		_quadBuffer->SetRenderTargets(_deferredBuffers->GetDepthStencilView());

		UINT skyBoxIndex = ObjectManager::Instance().GetCubeMapList().size() - 1;
		ObjectManager::Instance().GetCubeMapList()[skyBoxIndex]->Render();
	}
}
