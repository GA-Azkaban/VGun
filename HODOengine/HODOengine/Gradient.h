#pragma once
#include <map>
#include <vector>
#include <DirectXMath.h>

namespace HDData
{
	struct GradientColorKey
	{
		DirectX::XMINT3 color;
		float time; // 0.0 ~ 1.0
	};

	struct GradientAlphaKey
	{
		float alpha; // 0.0 ~ 1.0
		float time; // 0.0 ~ 1.0
	};

	class Gradient
	{
	public:
		friend class ColorOverLifetime;
		Gradient();
		~Gradient();

		DirectX::XMINT4 Evaluate(float time);
		void SetKeys(std::vector<GradientColorKey> colorKeys, std::vector<GradientAlphaKey> alphaKeys);

	private:
		std::vector<GradientColorKey> colorKeys;
		std::vector<GradientAlphaKey> alphaKeys;
	};
}
