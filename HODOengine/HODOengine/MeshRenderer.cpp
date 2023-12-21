#include "MeshRenderer.h"
#include "Transform.h"
#include "GraphicsObjFactory.h"
#include "RenderSystem.h"

namespace HDData
{
	MeshRenderer::MeshRenderer()
		: _staticMesh(HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateStaticMeshObject())
	{
		HDEngine::RenderSystem::Instance().PushRenderComponent(this);
		_renderable = _staticMesh;
	}

	HDEngine::IStaticMesh& MeshRenderer::Get()
	{
		return *_staticMesh;
	}
}