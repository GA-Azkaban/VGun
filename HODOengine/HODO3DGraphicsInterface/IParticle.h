#pragma once
#include "IRenderable.h"
#include "IMaterial.h"

namespace HDEngine
{
	class IParticle : IRenderable
	{
	public:
		virtual ~IParticle() {}
		virtual void SetMesh(const std::string& meshName) = 0;
		virtual void SetMaterial(HDEngine::IMaterial* material) = 0;
	};
}
