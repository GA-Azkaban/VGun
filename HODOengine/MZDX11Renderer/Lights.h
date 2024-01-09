#pragma once
#include <DirectXMath.h>

struct DirectionalLight
{
	DirectX::XMFLOAT4 Color;
	DirectX::XMFLOAT3 Direction;
};

struct PointLight
{
	DirectX::XMFLOAT4 Color;
	DirectX::XMFLOAT4 Position;
};

struct SpotLight
{
	DirectX::XMFLOAT4 Color;
	DirectX::XMFLOAT4 Position;
	DirectX::XMFLOAT3 Direction;
	float SpotPower;
};