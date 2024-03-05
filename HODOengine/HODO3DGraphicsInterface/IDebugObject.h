#pragma once
#include <string>
#include "IRenderable.h"
#include "MathHeader.h"

namespace HDEngine
{
	class IDebugObject : public IRenderable
	{
	public:
		virtual ~IDebugObject() {};
		virtual void SetFillModeSolid() = 0;
		virtual void SetFillModeWireframe() = 0;
		virtual void SetColor(const Vector4& color) = 0;
		virtual void LoadMesh(const std::string& meshName) = 0;
	};
}
