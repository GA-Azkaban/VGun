#pragma once
#include <vector>

#include "Component.h"
#include "Transform.h"
#include "..\\HODO3DGraphicsInterface\\ISketchable.h"

namespace HDData
{
	class UIBase : public Component
	{
	public:
		UIBase();
		void UpdateRenderData();
		virtual void OnUpdateTransform()
		{
			for (auto& sketch : _sketchable)
			{
				sketch->SetWorldTM(GetTransform()->GetWorldTM());
			}
		}

		std::vector<HDEngine::ISketchable*> _sketchable;

		// HDEngine::ISketchable* _sketchable;
	};
}
