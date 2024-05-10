#include "ParticlePool.h"
#include "../HODO3DGraphicsInterface/IParticle.h"
#include "GraphicsObjFactory.h"

namespace HDEngine
{
	
	ParticlePool::ParticlePool()
	{
		for (int i = 0; i < 20; ++i)
		{
			IParticle* newParticle = GraphicsObjFactory::Instance().GetFactory()->CreateParticle();
			_particles.push_back(newParticle);
		}
	}

	HDEngine::IParticle* ParticlePool::SummonParticle()
	{
		if (_particles.empty())
		{	
			IParticle* newParticle = GraphicsObjFactory::Instance().GetFactory()->CreateParticle();
			_particles.push_back(newParticle);		
		}
		IParticle* ret = _particles.back();
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
