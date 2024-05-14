#pragma once
#include "AnimationCurve.h"
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

	class MinMaxCurve
	{
	public:
		MinMaxCurve(float constant)
			: _constant(constant), _mode(ParticleSystemCurveMode::Constant),
			rd(), gen(rd)
		{

		}
		MinMaxCurve(float min, float max)
			: _constantMin(min), _constantMax(max), _mode(ParticleSystemCurveMode::TwoConstants),
			rd(), gen(rd)
		{

		}
		MinMaxCurve(float multiplier, AnimationCurve curve)
			: _curveMultiplier(multiplier), _curve(curve), _mode(ParticleSystemCurveMode::Curve),
			rd(), gen(rd)
		{

		}

		float Evaluate(float time)
		{
			float ret;
			switch (_mode)
			{
				case HDData::ParticleSystemCurveMode::Constant:
					ret = _constant;
					break;
				case HDData::ParticleSystemCurveMode::Curve:
					ret = _curve.Evaluate(time);
					break;
				case HDData::ParticleSystemCurveMode::TwoConstants:
					std::uniform_real_distribution dist(_constantMin, _constantMax);
					ret = dist(gen);
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

		// random		
		std::random_device rd;
		std::mt19937 gen;
	};
}
