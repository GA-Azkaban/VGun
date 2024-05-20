#pragma once
#include <map>
#include <vector>
#include <DirectXMath.h>
#include "dllExporter.h"

namespace HDData
{
	struct GradientColorKey
	{
		DirectX::XMINT3 color; // 0 ~ 255
		float time; // 0.0 ~ 1.0
	};

	struct GradientAlphaKey
	{
		int alpha; // 0 ~ 255
		float time; // 0.0 ~ 1.0
	};

	class HODO_API Gradient
	{
	public:
		Gradient();
		~Gradient();

		DirectX::XMINT4 Evaluate(float time);
		void SetKeys(std::vector<GradientColorKey> colorKeys, std::vector<GradientAlphaKey> alphaKeys);

	private:
		std::vector<GradientColorKey> colorKeys;
		std::vector<GradientAlphaKey> alphaKeys;
	};
}
