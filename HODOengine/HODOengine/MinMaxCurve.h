#pragma once
#include "dllExporter.h"
#include "AnimationCurve.h"
#include "RandomGenerator.h"
#include <random>
#include <utility>

namespace HDData
{
	enum class ParticleSystemCurveMode
	{
		Constant,
		Curve,
		TwoConstants
	};

	class HODO_API MinMaxCurve
	{
	public:
		MinMaxCurve(float constant)
			: _constant(constant), _mode(ParticleSystemCurveMode::Constant)
		{

		}
		MinMaxCurve(float min, float max)
			: _constantMin(min), _constantMax(max), _mode(ParticleSystemCurveMode::TwoConstants)
		{

		}
		MinMaxCurve(float multiplier, AnimationCurve curve)
			: _curveMultiplier(multiplier), _curve(curve), _mode(ParticleSystemCurveMode::Curve)
		{

		}

		float Evaluate(float time)
		{
			float ret = _constant;
			switch (_mode)
			{
				case HDData::ParticleSystemCurveMode::Constant:
					ret = _constant;
					break;
				case HDData::ParticleSystemCurveMode::Curve:
					ret = _curve.Evaluate(time);
					break;
				case HDData::ParticleSystemCurveMode::TwoConstants:
				{
					std::uniform_real_distribution dist(_constantMin, _constantMax);
					ret = dist(HDEngine::RandomGenerator::Instance().MersenneTwiste());
				}
					break;
				default:
					break;
			}

			return ret;
		}

	private:
		float _constant;
		float _constantMin;
		float _constantMax;
		float _curveMultiplier;
		AnimationCurve _curve;
		ParticleSystemCurveMode _mode;
	};
}
