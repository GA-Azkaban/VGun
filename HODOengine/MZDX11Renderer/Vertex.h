#pragma once
//***************************************************************************************
// A custom vertex definition
//***************************************************************************************

#include <DirectXMath.h>

namespace VertexStruct
{
	struct Vertex
	{
		DirectX::XMFLOAT3 Position;	// The position of the vertex
		DirectX::XMFLOAT2 UV;		// UV Coordinate for texturing
		DirectX::XMFLOAT3 Normal;	// Normal for lighting
		DirectX::XMFLOAT3 Tangent;	// For normal mapping

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
		DirectX::XMFLOAT2 UV;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT3 Tangent;
		float Weights0;
		float Weights1;
		float Weights2;
		unsigned int BoneIndices0;
		unsigned int BoneIndices1;
		unsigned int BoneIndices2;
		unsigned int BoneIndices3;
	};

	struct PosColor		// For debug object
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT4 Color;
	};
}

