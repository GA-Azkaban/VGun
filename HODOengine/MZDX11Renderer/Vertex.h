#pragma once
//***************************************************************************************
// Vertex.h by Frank Luna (C) 2011 All Rights Reserved.
//
// Defines vertex structures and input layouts.
//***************************************************************************************

#include <DirectXMath.h>


namespace VertexStruct
{
	struct PosColor
	{
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT4 Color;
	};

	struct PosNormal
	{
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT3 Normal;
	};

	struct PosTex
	{
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT2 Tex;
	};

	/// 텍스쳐 예제로 추가. 이름은 용책과의 연관성을 위해서 그냥 유지함
	struct Basic32
	{
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT2 Tex;
	};

	struct NormalMap
	{
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT2 Tex;
		DirectX::XMFLOAT3 TangentU;
	};

	struct PosNormalTexTanSkinned
	{
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT2 Tex;
		DirectX::XMFLOAT3 TangentU;
		float Weights0;
		float Weights1;
		float Weights2;
		unsigned int BoneIndices0;
		unsigned int BoneIndices1;
		unsigned int BoneIndices2;
		unsigned int BoneIndices3;
	};
}
