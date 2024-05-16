#pragma once
#include "../HODO3DGraphicsInterface/ParticleSystemRenderMode.h"
#include "HDMaterial.h"
#include "MaterialManager.h"
#include <string>

namespace HDData
{
	class RendererModule
	{
	public:
		RendererModule()
			: renderMode(HDEngine::ParticleSystemRenderMode::Billboard),
			material(HDEngine::MaterialManager::Instance().GetMaterial("Default-ParticleSystem")), mesh(""),
			minParticleSize(0.0f), maxParticleSize(0.5f)
		{
			
		}

	public:
		HDEngine::ParticleSystemRenderMode renderMode;
		HDData::Material* material;
		std::string mesh;
		float minParticleSize;
		float maxParticleSize;
	};
}
