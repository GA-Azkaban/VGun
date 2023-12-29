#pragma once
#include <DirectXMath.h>

struct DirectionalLight
{
	DirectX::XMFLOAT4 Color;
	DirectX::XMFLOAT3 Direction;

	void SetDirectionalLightValues(DirectX::XMFLOAT4 color, DirectX::XMFLOAT3 direction)
	{
		Color = color;
		Direction = direction;
	}
};

struct PointLight
{
	DirectX::XMFLOAT4 Color;
	DirectX::XMFLOAT3 Position;

	void SetPointLightValues(DirectX::XMFLOAT4 color, DirectX::XMFLOAT3 position)
	{
		Color = color;
		Position = position;
	}
};

struct SpotLight
{
	DirectX::XMFLOAT4 Color;
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Direction;
	float SpotPower;

	void SetSpotLightValues(DirectX::XMFLOAT4 color, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 direction, float spotPower)
	{
		Color = color;
		Position = position;
		Direction = direction;
		SpotPower = spotPower;
	}
};