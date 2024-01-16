#pragma once
#include "IRenderable.h"
#include "../HODOmath/HODOmath.h"
#include <string>

namespace HDEngine
{
	class IDebugObject : public IRenderable
	{
	public:
		virtual ~IDebugObject() {};
		virtual void SetFillModeSolid() = 0;
		virtual void SetFillModeWireframe() = 0;
		virtual void SetColor(const HDMath::HDFLOAT4& color) = 0;
		virtual void SetMesh(const std::string& meshName) = 0;
	};
}