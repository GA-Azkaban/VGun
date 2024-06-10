#pragma once
#include <string>
#include "IRenderable.h"
#include "IMaterial.h"

namespace HDEngine
{
	class IDecal : public IRenderable
	{
	public:
		virtual void LoadMaterial(HDEngine::IMaterial* material) = 0;
		virtual void SetColor(UINT r, UINT g, UINT b, UINT a) = 0;
	};
}
