#pragma once
#include <DirectXMath.h>

/// 2024.01.15 김민정
/// 우선 라이트에 대한 정보를 구조체로 만들어 넣었다.
/// 임시로 만든 것이며,
/// 추후에 라이트에 관련된 오브젝트를 만들어야 할 것이다.
/// 

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