#pragma once
#include <vector>

#include "Component.h"
#include "..\\HODO3DGraphicsInterface\\ISketchable.h"

namespace HDData
{
	class UIBase : public Component
	{
	public:
		UIBase();
		void UpdateRenderData();

		std::vector<HDEngine::ISketchable*> _sketchable;

		// HDEngine::ISketchable* _sketchable;
	};
}
