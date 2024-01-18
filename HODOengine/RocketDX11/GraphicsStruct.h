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


	/// 2024.01.15 김민정
	struct Vertex
	{
		DirectX::XMFLOAT3 Position;	// The position of the vertex
		DirectX::XMFLOAT3 Normal;	// Normal for lighting
		DirectX::XMFLOAT3 Tangent;	// For normal mapping
		DirectX::XMFLOAT2 UV;		// UV Coordinate for texturing

		Vertex() {}
		Vertex(const DirectX::XMFLOAT3& p, const DirectX::XMFLOAT2& uv, const DirectX::XMFLOAT3& n, const DirectX::XMFLOAT3& t)
			: Position(p), UV(uv), Normal(n), Tangent(t) {}
		Vertex(
			float px, float py, float pz,
			float u, float v,
			float nx, float ny, float nz,
			float tx, float ty, float tz)
			: Position(px, py, pz), UV(u, v),
			Normal(nx, ny, nz), Tangent(tx, ty, tz) {}
		Vertex(const DirectX::XMFLOAT3& p, const DirectX::XMFLOAT2& uv)
			: Position(p), UV(uv), Normal{ 0, 0, 0 }, Tangent{ 0, 0, 0 } {}
	};

	struct VertexSkinning
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT3 Tangent;
		DirectX::XMFLOAT2 UV;
		DirectX::XMFLOAT4 Weights;
		DirectX::XMUINT4 BoneIndices;
	};

	struct PosColor		// For debug object
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT4 Color;
	};
}