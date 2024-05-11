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

		HDEngine::ParticleSystemRenderMode GetRenderMode() const;
		HDData::Material* GetMaterial() const;
		std::string GetMesh() const;
		float GetMinParticleSize() const;
		float GetMaxParticleSize() const;

		void SetRenderMode(HDEngine::ParticleSystemRenderMode renderMode);
		void SetMaterial(HDData::Material* material);
		void SetMesh(std::string meshName);
		void SetParticleSize(float minSize, float maxSize);

	protected:
		virtual void Update() override;

	private:
		HDEngine::ParticleSystemRenderMode _renderMode;
		HDData::Material* _material;
		std::string _mesh;
		float _minParticleSize;
		float _maxParticleSize;
	};
}
