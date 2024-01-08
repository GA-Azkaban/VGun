#pragma once
#include "Component.h"
#include "..\\HODO3DGraphicsInterface\\ISketchable.h"

namespace HDData
{
	class UIBase : public Component
	{
	public:
		UIBase();
		void UpdateRenderData();

		HDEngine::ISketchable* _sketchable;
	};
}
