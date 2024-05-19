#pragma once
#include "Component.h"
#include "dllExporter.h"
#include "MainModule.h"
#include "ColorOverLifetimeModule.h"
#include "EmssionModule.h"
#include "LimitVelocityOverLifetimeModule.h"
#include "SizeOverLifetimeModule.h"
#include "RotationOverLifetimeModule.h"
#include "RendererModule.h"
#include <DirectXMath.h>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace HDEngine
{
	class IParticle;
	class IParticleSystem;
}

namespace HDData
{

	class Material;
	class Particle;
	class ParticleSystemRenderer;

	class HODO_API ParticleSystem : public Component
	{
	public:
		ParticleSystem();
		~ParticleSystem();

		void Play();
		void Stop();
		void Clear();
		bool IsPlaying() const;

		std::unordered_map<HDEngine::IParticle*, std::pair<float, float>>& GetActivatedParticleList();

	protected:
		virtual void OnEnable() override;
		virtual void OnDisable() override;
		virtual void Start() override;
		virtual void Update() override;

	public:
		MainModule main;
		ColorOverLifetimeModule colorOverLifetime;
		EmissionModule emission;
		//LimitVelocityOverLifetimeModule limitVelocityOverLifetime;
		SizeOverLifetimeModule sizeOverLifetime;
		RotationOverLifetimeModule rotationOverLifetime;
		RendererModule rendererModule;
		bool useAutoRandomSeed;
		float time;

	private:
		HDEngine::IParticleSystem* _particleSystem;
		bool _isPlaying;
		float _accumulatedDeltaTime;
		// <Particle, <lifetime, accumulateDeltaTime>>
		std::unordered_map<HDEngine::IParticle*, std::pair<float, float>> _activatedParticles;
		std::vector<HDEngine::IParticle*> _lifeOverParticles;

		// random
		std::mt19937 _gen;

	};

}
