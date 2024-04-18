#include "SkyboxPass.h"
#include "DeferredBuffers.h"
#include "QuadBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ResourceManager.h"
#include "Skybox.h"

SkyboxPass::SkyboxPass(DeferredBuffers* deferredBuffers, QuadBuffer* quadBuffer)
	: _deferredBuffers(deferredBuffers), _quadBuffer(quadBuffer)
{

}

void SkyboxPass::Render()
{
	_quadBuffer->SetRenderTargets(_deferredBuffers->GetDepthStencilView());

	UINT skyBoxIndex = Skybox::_skyboxObjects.size() - 1;
	Skybox::_skyboxObjects[skyBoxIndex]->Render();
}
