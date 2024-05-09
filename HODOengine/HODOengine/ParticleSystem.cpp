#include "ParticleSystem.h"
#include "GameObject.h"
#include "ParticleSystemRenderer.h"

namespace HDData
{

	ParticleSystem::ParticleSystem()
		: main(), colorOverLifetime(), emission(), 
		limitVelocityOverLifetime(), sizeOverLifetime(), rotationOverLifetime(),
		useAutoRandomSeed(true), particleCount(0), time(0.0f)
	{
		renderer = GetGameObject()->AddComponent<ParticleSystemRenderer>();
	}

	ParticleSystem::~ParticleSystem()
	{

	}

	void ParticleSystem::Play()
	{

	}

	void ParticleSystem::Clear()
	{

	}

	void ParticleSystem::Update()
	{

	}

}
