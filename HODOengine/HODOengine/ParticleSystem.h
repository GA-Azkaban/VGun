#pragma once
#include "Component.h"
#include "dllExporter.h"
#include "MainModule.h"
#include "ColorOverLifetimeModule.h"
#include "EmssionModule.h"
#include "LimitVelocityOverLifetimeModule.h"
#include "SizeOverLifetimeModule.h"
#include "RotationOverLifetimeModule.h"
#include "../HODO3DGraphicsInterface/IParticle.h"
#include <DirectXMath.h>
#include <functional>
#include <string>
#include <unordered_set>
#include <random>

namespace HDData
{

	class Material;
	class ParticleSystemRenderer;

	class HODO_API ParticleSystem : public Component
	{
	public:
		ParticleSystem();
		~ParticleSystem();

		void Play();
		void Stop();
		void Clear();

	protected:
		virtual void Update() override;

	public:
		MainModule main;
		ColorOverLifetimeModule colorOverLifetime;
		EmissionModule emission;
		LimitVelocityOverLifetimeModule limitVelocityOverLifetime;
		SizeOverLifetimeModule sizeOverLifetime;
		RotationOverLifetimeModule rotationOverLifetime;
		bool useAutoRandomSeed;
		float time;
		//int particleCount;

	private:
		ParticleSystemRenderer* _renderer;
		bool _isPlaying;
		float _accumulatedDeltaTime;

		std::unordered_set<HDEngine::IParticle*> _activatedParticles;

		// random		
		std::random_device rd;
		std::mt19937 gen;
	};

}
