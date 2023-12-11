#pragma once
#include "IRenderable.h"
#include "../HODOmath/HODOmath.h"

namespace hodoGI
{
	class IDebugObject : public IRenderable
	{
	public:
		virtual ~IDebugObject() {};
		virtual void SetFillModeSolid() = 0;
		virtual void SetFillModeWireframe() = 0;
		virtual void SetColor(const HDMaths::HDFLOAT4& color) = 0;
	};
}