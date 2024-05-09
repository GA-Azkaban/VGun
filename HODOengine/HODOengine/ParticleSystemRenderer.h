#pragma once
#include "RendererBase.h"
#include "dllExporter.h"
#include "ParticleSystemRenderMode.h"
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
		virtual void OnEnable() override;
		virtual void OnDisable() override;

	public:
		ParticleSystemRenderMode renderMode;
		HDData::Material* material;
		std::string mesh;
		float minParticleSize;
		float maxParticleSize;
	};
}
