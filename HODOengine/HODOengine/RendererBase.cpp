#include "RendererBase.h"
#include "Transform.h"

namespace HDData
{
	RendererBase::RendererBase()
		:_renderable(nullptr)
	{

	}

	void RendererBase::UpdateRenderData()
	{
		_renderable->SetWorldTM(GetTransform()->GetWorldTM());
	}

// 	void RendererBase::OnEnable()
// 	{
// 		_renderable->SetActive(true);
// 	}
// 
// 	void RendererBase::OnDisable()
// 	{
// 		_renderable->SetActive(false);
// 	}
}
