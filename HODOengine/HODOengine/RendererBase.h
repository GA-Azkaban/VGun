#pragma once
#include "Component.h"
#include "..\\HODO3DGraphicsInterface\\IRenderable.h"
#include <unordered_map>
#include <memory>

namespace HDData
{
	class RendererBase : public Component
	{
	public:
		RendererBase(HDEngine::IRenderable* renderable);
		virtual void Update() override;
		static std::unordered_map<HDEngine::IRenderable*, RendererBase*> _renderableMap;

	protected:
		virtual void OnEnable();
		virtual void OnDisable();

		std::unique_ptr<HDEngine::IRenderable> _renderable;
	};
}

