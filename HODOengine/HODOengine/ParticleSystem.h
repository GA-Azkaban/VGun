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
#include <random>
#include <utility>

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

		std::unordered_map<HDData::Particle*, std::pair<float, float>>& GetActivatedParticleList();

	protected:
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
		//int particleCount;

	private:
		bool _isPlaying;
		float _accumulatedDeltaTime;

		// <Particle, <lifetime, accumulateDeltaTime>>
		std::unordered_map<HDData::Particle*, std::pair<float, float>> _activatedParticles;

		std::vector<HDData::Particle*> _lifeOverParticles;

		// random		
		std::random_device rd;
		std::mt19937 gen;
	};

}
