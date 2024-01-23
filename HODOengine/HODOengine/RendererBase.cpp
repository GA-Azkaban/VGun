#include "RendererBase.h"
#include "RenderSystem.h"

namespace HDData
{
	RendererBase::RendererBase()
	{
		HDEngine::RenderSystem::Instance().PushRenderComponent(this);
	}
}
