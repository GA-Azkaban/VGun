#pragma once
#include <DirectXMath.h>

namespace RocketCore::Graphics
{
	enum class VertexType
	{
		COLOR_VERTEX,
		TEXTURE_VERTEX
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

	struct MatrixBufferType
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};
}