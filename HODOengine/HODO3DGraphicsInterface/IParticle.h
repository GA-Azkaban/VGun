#pragma once
#include "IRenderable.h"
#include "IMaterial.h"

namespace HDEngine
{
	enum class ShapeType
	{
		Cone,
	};

	enum class RenderType
	{
		Billboard,
		Mesh,
	};

	struct BurstsDesc
	{
		float Time = 0.0f;
		int Count = 0;
		int MinCount = -1;
		int MaxCount = -1;
		int Cycles = 1;
		float Interval = 0.0f;
		float Probability = 1.0f;
	};

	class IParticle : IRenderable
	{
	public:
		virtual ~IParticle() {}

		virtual void SetDuration(float duration) = 0;
		virtual void SetLoop(bool isLoop) = 0;
		virtual void SetStartDelayConstant(float value) = 0;
		virtual void SetStartDelayRandomBetweenTwoConstants(float minValue, float maxValue) = 0;
		virtual void SetStartLifetimeConstant(float value) = 0;
		virtual void SetStartLifetimeRandomBetweenTwoConstants(float minValue, float maxValue) = 0;
		virtual void SetStartSpeedConstant(float value) = 0;
		virtual void SetStartSpeedRandomBetweenTwoConstants(float minValue, float maxValue) = 0;
		virtual void SetStartSizeConstant(float value) = 0;
		virtual void SetStartSizeRandomBetweenTwoConstants(float minValue, float maxValue) = 0;
		virtual void SetStartRotationConstant(float degree) = 0;
		virtual void SetStartRotationRandomBetweenTwoConstants(float minDegree, float maxDegree) = 0;
		virtual void SetStartColor(DirectX::XMINT4 color) = 0;
		virtual void SetStartColorRandomBetweenTwoColors(DirectX::XMINT4 color1, DirectX::XMINT4 color2) = 0;
		virtual void SetMaxParticles(unsigned int value) = 0;
		virtual void SetAutoRandomSeed(bool isRandom) = 0;

		// Emission
		virtual void SetRateOverTimeConstant(unsigned int value) = 0;
		virtual void SetRateOverTimeRandomBetweenTwoConstants(unsigned int minValue, unsigned int maxValue) = 0;
		virtual void SetBursts(HDEngine::BurstsDesc burstsDesc) = 0;

		// Shape
		virtual void SetShape(HDEngine::ShapeType shapeType) = 0;
		virtual void SetAngle(float angle) = 0;
		virtual void SetRadius(float radius) = 0;

		// Limit Velocity over Lifetime
		virtual void SetSpeed(float value) = 0;
		virtual void SetDampen(float value) = 0;

		// Color over Lifetime
		virtual void SetColorGradient(std::map<float, DirectX::XMINT4> gradient) = 0;

		// Size over Lifetime
		virtual void SetSize(std::function<float(float)> curve) = 0;

		// Rotation over Lifetime
		virtual void SetAngularVelocity(float value) = 0;

		// Renderer
		virtual void SetRenderMode(HDEngine::RenderType type) = 0;
		virtual void SetMesh(const std::string& meshName) = 0;
		virtual void SetMaterial(HDEngine::IMaterial* material) = 0;
	};
}
