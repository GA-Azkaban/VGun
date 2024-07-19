#pragma once
#include "ParticleSystemRenderMode.h"
#include "IMaterial.h"
#include "IParticle.h"
#include <string>

namespace HDEngine
{
	class IParticleSystem
	{
	public:
		virtual void SetActive(bool isActive) = 0;
		virtual void SetMesh(const std::string& meshName) = 0;
		virtual void SetMaterial(HDEngine::IMaterial* material) = 0;
		virtual void SetRenderMode(HDEngine::ParticleSystemRenderMode renderMode) = 0;
		virtual void SetAlphaBlend(bool isAlphaBlend) = 0;
		virtual IParticle* SummonParticle() = 0;
		virtual void DestroyParticle(IParticle* particle) = 0;
	};
}
