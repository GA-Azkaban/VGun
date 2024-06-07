#pragma once
#include <string>
#include "IRenderable.h"

namespace HDEngine
{
	class IDecal : public IRenderable
	{
	public:
		virtual void LoadTexture(const std::string& fileName) = 0;
		virtual void SetColor(UINT r, UINT g, UINT b, UINT a) = 0;
	};
}
