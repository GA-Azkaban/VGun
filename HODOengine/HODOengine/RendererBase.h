#pragma once
#include <memory>

#include "Component.h"
#include "..\\HODO3DGraphicsInterface\\IRenderable.h"

namespace HDData
{
	class RendererBase : public Component
	{
	public:
		RendererBase();
		void UpdateRenderData();

	protected:
		virtual void OnEnable();
		virtual void OnDisable();

		std::unique_ptr<HDEngine::IRenderable> _renderable;
	};
}

