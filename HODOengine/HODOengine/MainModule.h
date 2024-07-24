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
		float gravityModifier = 0.0f;
		// 중력. 임시코드.
		// 이건 원래대로라면 Velocity 설정하는 곳에서 y 속도 값을 받아야한다.
		float initialVelocity = 0.0f;
	};
}
