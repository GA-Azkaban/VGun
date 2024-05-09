#pragma once
#include "Component.h"
#include "dllExporter.h"
#include "MainModule.h"
#include "ColorOverLifetimeModule.h"
#include "EmssionModule.h"
#include "LimitVelocityOverLifetimeModule.h"
#include "SizeOverLifetimeModule.h"
#include "RotationOverLifetimeModule.h"
#include <DirectXMath.h>
#include <functional>
#include <string>
#include <map>
#include <vector>
#include "../HODO3DGraphicsInterface/IParticle.h"

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
		int particleCount;
		float time;

	private:
		ParticleSystemRenderer* renderer;
	};
}
