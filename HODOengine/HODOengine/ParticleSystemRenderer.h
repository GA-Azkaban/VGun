#pragma once
#include "RendererBase.h"

namespace HDData
{
	class ParticleSystem;

	class ParticleSystemRenderer : public RendererBase
	{
	public:
		ParticleSystemRenderer();
		~ParticleSystemRenderer();

	protected:
		virtual void Start() override;
		virtual void Update() override;

	private:
		ParticleSystem* _particleSystem;
	};
}
