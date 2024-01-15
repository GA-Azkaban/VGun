#pragma once
#include <DirectXMath.h>

namespace RocketCore::Graphics
{
	enum class VertexType
	{
		COLOR_VERTEX,
		TEXTURE_VERTEX,
		LIGHT_VERTEX
	};

	struct ColorVertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};

	struct TextureVertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 uv;
	};

	struct LightVertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 uv;
		DirectX::XMFLOAT3 normal;
	};

	struct MatrixBufferType
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

	struct LightBufferType
	{
		DirectX::XMFLOAT4 diffuseColor;
		DirectX::XMFLOAT3 lightDirection;
		float padding;	// 구조체가 CreateBuffer 함수 요구 사항에 대해 16의 배수가 되도록 여분의 패딩을 추가
	};
}