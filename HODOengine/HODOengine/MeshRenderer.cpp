#include "MeshRenderer.h"
#include "GraphicsObjFactory.h"

namespace HDData
{
	MeshRenderer::MeshRenderer()
		: _staticMesh(HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateStaticMeshObject())
	{
		
	}

	HDEngine::IStaticMesh& MeshRenderer::Get()
	{
		return *_staticMesh;
	}
}