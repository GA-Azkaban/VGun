#include "DebugMeshPass.h"
#include "DeferredBuffers.h"
#include "QuadBuffer.h"
#include "ObjectManager.h"
#include "ResourceManager.h"
#include "Camera.h"
#include "HelperObject.h"
#include "../HODO3DGraphicsInterface/PrimitiveHeader.h"

namespace RocketCore::Graphics
{
	DebugMeshPass::DebugMeshPass(DeferredBuffers* deferredBuffers, QuadBuffer* quadBuffer)
		: _deferredBuffers(deferredBuffers), _quadBuffer(quadBuffer)
	{

	}

	void DebugMeshPass::Render()
	{
		_quadBuffer->SetRenderTargets(_deferredBuffers->GetDepthStencilView());

#ifdef _DEBUG
		Camera* cam = Camera::GetMainCamera();

		for (auto e : ObjectManager::Instance().GetCubePrimitiveList())
		{
			e->worldTM.m[0][0] *= e->widthHeightDepth.x;
			e->worldTM.m[1][1] *= e->widthHeightDepth.y;
			e->worldTM.m[2][2] *= e->widthHeightDepth.z;

			ResourceManager::Instance().GetCubePrimitive()->Draw(e->worldTM, cam->GetViewMatrix(), cam->GetProjectionMatrix(), e->color, nullptr, true);
		}

		for (auto e : ObjectManager::Instance().GetSpherePrimitiveList())
		{
			e->worldTM.m[0][0] *= e->diameter;
			e->worldTM.m[1][1] *= e->diameter;
			e->worldTM.m[2][2] *= e->diameter;

			ResourceManager::Instance().GetSpherePrimitive()->Draw(e->worldTM, cam->GetViewMatrix(), cam->GetProjectionMatrix(), e->color, nullptr, true);
		}

		for (auto e : ObjectManager::Instance().GetCylinderPrimitiveList())
		{
			e->worldTM.m[0][0] *= e->diameter;
			e->worldTM.m[1][1] *= e->height / 2;
			e->worldTM.m[2][2] *= e->diameter;

			ResourceManager::Instance().GetCylinderPrimitive()->Draw(e->worldTM, cam->GetViewMatrix(), cam->GetProjectionMatrix(), e->color, nullptr, true);
		}

		for (auto& helperObject : ObjectManager::Instance().GetHelperObjList())
		{
			helperObject->Render();
		}
#endif
	}
}
