#pragma once
#include "Singleton.h"
#include <vector>

namespace HDEngine
{
	class IParticle;

	class ParticlePool : public Singleton<ParticlePool>
	{
	public:
		friend Singleton;

		ParticlePool();

		HDEngine::IParticle* SummonParticle();
		void Retrieve(HDEngine::IParticle* particle);

	private:
		std::vector<HDEngine::IParticle*> _particles;
	};
}
