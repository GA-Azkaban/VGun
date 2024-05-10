#pragma once
#include "RendererBase.h"
#include "dllExporter.h"
#include "../HODO3DGraphicsInterface/ParticleSystemRenderMode.h"
#include "HDMaterial.h"
#include <string>

namespace HDData
{
	class HODO_API ParticleSystemRenderer : public RendererBase
	{
	public:
		ParticleSystemRenderer();
		~ParticleSystemRenderer();

	protected:
		virtual void Update() override;

	public:
		HDEngine::ParticleSystemRenderMode renderMode;
		HDData::Material* material;
		std::string mesh;
		float minParticleSize;
		float maxParticleSize;
	};
}
