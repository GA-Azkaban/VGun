#pragma once
#include "Component.h"
#include "dllExporter.h"
#include <DirectXMath.h>
#include <functional>
#include <string>
#include <map>
#include <vector>
#include "../HODO3DGraphicsInterface/IParticle.h"

namespace HDData
{
	struct Gradient
	{
		std::vector<std::map<
	};

	struct ColorOverLifetime
	{
		Gradient color;
		bool enabled;
	};

	class Material;

	class HODO_API ParticleSystem : public Component
	{
	public:
		ParticleSystem();
		~ParticleSystem();

		void SetDuration(float duration);
		void SetLoop(bool isLoop);
		void SetStartDelayConstant(float value);
		void SetStartDelayRandomBetweenTwoConstants(float minValue, float maxValue);
		void SetStartLifetimeConstant(float value);
		void SetStartLifetimeRandomBetweenTwoConstants(float minValue, float maxValue);
		void SetStartSpeedConstant(float value);
		void SetStartSpeedRandomBetweenTwoConstants(float minValue, float maxValue);
		void SetStartSizeConstant(float value);
		void SetStartSizeRandomBetweenTwoConstants(float minValue, float maxValue);
		void SetStartRotationConstant(float degree);
		void SetStartRotationRandomBetweenTwoConstants(float minDegree, float maxDegree);
		void SetStartColor(DirectX::XMINT4 color);
		void SetStartColorRandomBetweenTwoColors(DirectX::XMINT4 color1, DirectX::XMINT4 color2);
		void SetMaxParticles(unsigned int value);
		void SetAutoRandomSeed(bool isRandom);

		// Emission
		void SetRateOverTimeConstant(unsigned int value);
		void SetRateOverTimeRandomBetweenTwoConstants(unsigned int minValue, unsigned int maxValue);
		void SetBursts(HDEngine::BurstsDesc burstsDesc);

		// Shape
		void SetShape(HDEngine::ShapeType shapeType);
		void SetAngle(float angle);
		void SetRadius(float radius);

		// Limit Velocity over Lifetime
		void SetSpeed(float value);
		void SetDampen(float value);

		// Color over Lifetime
		void SetColorGradient(std::map<float, DirectX::XMINT4> gradient);

		// Size over Lifetime
		void SetSize(std::function<float(float)> curve);

		// Rotation over Lifetime
		void SetAngularVelocity(float value);

		// Renderer
		void SetRenderMode(HDEngine::RenderType type);
		void SetMesh(const std::string& meshName);
		void SetMaterial(HDData::Material* material);

	public:
		void Play();
		void Clear();

	public:


	};
}
