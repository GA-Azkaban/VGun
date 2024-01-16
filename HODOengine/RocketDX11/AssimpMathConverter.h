#pragma once
#include <DirectXMath.h>
#include <assimp/scene.h>

inline DirectX::XMMATRIX AIMatrix4x4ToXMMatrix(const aiMatrix4x4& mat)
{
	DirectX::XMMATRIX m;
	m.r[0].m128_f32[0] = mat.a1;
	m.r[0].m128_f32[1] = mat.a2;
	m.r[0].m128_f32[2] = mat.a3;
	m.r[0].m128_f32[3] = mat.a4;

	m.r[1].m128_f32[0] = mat.b1;
	m.r[1].m128_f32[1] = mat.b2;
	m.r[1].m128_f32[2] = mat.b3;
	m.r[1].m128_f32[3] = mat.b4;

	m.r[2].m128_f32[0] = mat.c1;
	m.r[2].m128_f32[1] = mat.c2;
	m.r[2].m128_f32[2] = mat.c3;
	m.r[2].m128_f32[3] = mat.c4;

	m.r[3].m128_f32[0] = mat.d1;
	m.r[3].m128_f32[1] = mat.d2;
	m.r[3].m128_f32[2] = mat.d3;
	m.r[3].m128_f32[3] = mat.d4;

	return m;
}

inline DirectX::XMFLOAT3 AIVec3ToXMFloat3(const aiVector3D& vec)
{
	DirectX::XMFLOAT3 v;

	v.x = vec.x;
	v.y = vec.y;
	v.z = vec.z;

	return v;
}

inline DirectX::XMFLOAT4 AIQuaternionToXMFloat4(const aiQuaternion& q)
{
	DirectX::XMFLOAT4 v;

	v.x = q.x;
	v.y = q.y;
	v.z = q.z;
	v.w = q.w;

	return v;
}