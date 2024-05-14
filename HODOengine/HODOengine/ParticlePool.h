#pragma once
#include "Singleton.h"
#include "Particle.h"
#include <vector>

namespace HDEngine
{
	class ParticlePool : public Singleton<ParticlePool>
	{
	public:
		friend Singleton;

		ParticlePool();

		HDData::Particle* SummonParticle();
		void Retrieve(HDData::Particle* particle);

	private:
		std::vector<HDData::Particle*> _particles;
	};
}
