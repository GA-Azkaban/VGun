#pragma once
#include "IRenderable.h"
#include "HodoGIColor.h"

namespace hodoGI
{
	class IDebugObject : public IRenderable
	{
	public:
		virtual ~IDebugObject() {};
		virtual void SetFillModeSolid() = 0;
		virtual void SetFillModeWireframe() = 0;
		virtual void SetColor(const hodoGI::Color& color) = 0;
	};
}