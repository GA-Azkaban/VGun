#include "GBufferPass.h"
#include "ObjectManager.h"
#include "StaticMeshObject.h"
#include "SkinningMeshObject.h"
#include "DeferredBuffers.h"

namespace RocketCore::Graphics
{
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

		for (auto staticMeshObj : ObjectManager::Instance().GetStaticMeshObjList())
		{
			staticMeshObj->Render();
		}

		for (auto skinningMeshObj : ObjectManager::Instance().GetSkinningMeshObjList())
		{
			skinningMeshObj->Render();
		}
	}
}
