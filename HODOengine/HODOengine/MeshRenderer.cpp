#include "MeshRenderer.h"
#include "Transform.h"

namespace HDData
{
	MeshRenderer::MeshRenderer()
		: RendererBase(nullptr//RenderSystem.CreateStaticMesh()
		)		
	{
		_staticMesh = std::make_unique<HDEngine::IStaticMesh>(dynamic_cast<HDEngine::IStaticMesh*>(RendererBase::_renderable.get()));
	}

	HDEngine::IStaticMesh& MeshRenderer::Get()
	{
		return *_staticMesh;
	}
}