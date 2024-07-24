#include "ParticlePool.h"
#include "Particle.h"

namespace RocketCore::Graphics
{

	ParticlePool::ParticlePool()
	{
		for (int i = 0; i < 20; ++i)
		{
			HDEngine::IParticle* newParticle = new Particle();
			_particles.push_back(newParticle);
		}
	}

	HDEngine::IParticle* ParticlePool::SummonParticle()
	{
		if (_particles.empty())
		{
			for (int i = 0; i < 10; ++i)
			{
				HDEngine::IParticle* newParticle = new Particle();
				_particles.push_back(newParticle);
			}
		}

		HDEngine::IParticle* ret = _particles.back();
		_particles.pop_back();
		return ret;
	}

	void ParticlePool::Retrieve(HDEngine::IParticle* particle)
	{
		if (particle != nullptr)
		{
			_particles.push_back(particle);
		}
	}

}
