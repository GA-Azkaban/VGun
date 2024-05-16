#pragma once
#include "Singleton.h"
#include "../HODO3DGraphicsInterface/IParticle.h"
#include <vector>

namespace RocketCore::Graphics
{
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
