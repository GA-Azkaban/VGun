#pragma once
#include "IRenderable.h"

namespace HDEngine
{
	class ISketchable : public IRenderable
	{
	public:
		virtual ~ISketchable() {};
		virtual void SetScereenSpace() = 0;
		virtual void SetWorldSpace() = 0;
	};
}