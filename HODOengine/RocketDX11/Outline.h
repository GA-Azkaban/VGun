#pragma once
#include <DirectXMath.h>

namespace RocketCore::Graphics
{
	struct Outline
	{
		bool isActive;
		bool depthCheck;
		DirectX::XMFLOAT4 color;
	};
}
