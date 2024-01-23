#include "SkinnedMeshRenderer.h"
#include "GraphicsObjFactory.h"

namespace HDData
{
	SkinnedMeshRenderer::SkinnedMeshRenderer()
		: _skinnedMesh(HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateSkinnedMeshObject())
	{

	}

	HDEngine::ISkinnedMesh& SkinnedMeshRenderer::Get()
	{
		return *_skinnedMesh;
	}
}