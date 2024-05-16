#pragma once
#include <DirectXMath.h>

namespace HDData
{
	class MainModule
	{
	public:
		float duration;
		bool loop;
		//float startDelay;
		float minStartLifetime;
		float maxStartLifetime;
		float minStartSpeed;
		float maxStartSpeed;
		float minStartSize;
		float maxStartSize;
		float minStartRotation;
		float maxStartRotation;
		DirectX::XMINT4 minStartColor;
		DirectX::XMINT4 maxStartColor;
		//unsigned int maxParticles;
	};
}
