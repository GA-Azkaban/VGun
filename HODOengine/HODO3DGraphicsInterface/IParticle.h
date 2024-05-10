#pragma once
#include "IRenderable.h"
#include "IMaterial.h"
#include "ParticleSystemRenderMode.h"

namespace HDEngine
{
	class IParticle : IRenderable
	{
	public:
		virtual ~IParticle() {}
		virtual void SetMesh(const std::string& meshName) = 0;
		virtual void SetMaterial(HDEngine::IMaterial* material) = 0;
		virtual void SetRenderMode(HDEngine::ParticleSystemRenderMode renderMode) = 0;
	};
}
