#pragma once
#include "Component.h"
#include "..\\HODO3DGraphicsInterface\\IRenderable.h"

namespace HDData
{
	class RendererBase : public Component
	{
	public:
		RendererBase();
		void UpdateRenderData();

// 	public:
// 		virtual void OnEnable() override;
// 		virtual void OnDisable() override;

		HDEngine::IRenderable* _renderable;
	};
}

