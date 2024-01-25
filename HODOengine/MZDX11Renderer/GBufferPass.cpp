#include "GBufferPass.h"
#include "MZDX11Renderer.h"
#include "IMeshObject.h"
#include "DeferredBuffers.h"

GBufferPass::GBufferPass(DeferredBuffers* deferredBuffers)
	: _deferredBuffers(deferredBuffers)
{

}

GBufferPass::~GBufferPass()
{
	delete _deferredBuffers;
}

void GBufferPass::Render()
{
	_deferredBuffers->SetRenderTargets();
	_deferredBuffers->ClearRenderTargets();

	for (auto& meshObject : IMeshObject::meshObjects)
	{
		meshObject->Render();
	}
}
