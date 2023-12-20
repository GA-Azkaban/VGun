#include "RendererBase.h"
#include "Transform.h"
#include "GraphicsObjFactory.h"
#include "..\\HODO3DGraphicsInterface\\IStaticMesh.h";

namespace HDData
{
	RendererBase::RendererBase()
		:_renderable(HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateStaticMeshObject())
	{

	}

	void RendererBase::UpdateRenderData()
	{
		_renderable->SetWorldTM(GetTransform()->GetWorldTM());
	}

	void RendererBase::OnEnable()
	{
		_renderable->SetActive(true);
	}

	void RendererBase::OnDisable()
	{
		_renderable->SetActive(false);
	}
}
