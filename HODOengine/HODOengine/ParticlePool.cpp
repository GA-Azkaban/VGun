#include "ParticlePool.h"
#include "Particle.h"
#include "GraphicsObjFactory.h"

namespace HDEngine
{

	ParticlePool::ParticlePool()
	{
		for (int i = 0; i < 20; ++i)
		{
			HDData::Particle* newParticle = new HDData::Particle();
			_particles.push_back(newParticle);
		}
	}

	HDData::Particle* ParticlePool::SummonParticle()
	{
		if (_particles.empty())
		{
			for (int i = 0; i < 10; ++i)
			{
				HDData::Particle* newParticle = new HDData::Particle();
				_particles.push_back(newParticle);
			}
		}

		HDData::Particle* ret = _particles.back();
		_particles.pop_back();
		return ret;
	}

	void ParticlePool::Retrieve(HDData::Particle* particle)
	{
		if (particle != nullptr)
		{
			HDEngine::GraphicsObjFactory::Instance().GetFactory()->DestroyParticle(&(particle->Get()));
			_particles.push_back(particle);
		}
	}

}
