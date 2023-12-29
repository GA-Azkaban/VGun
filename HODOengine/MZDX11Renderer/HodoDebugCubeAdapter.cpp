#include "HodoDebugCubeAdapter.h"
#include "HODOmath.h"
#include <DirectXMath.h>

void hodoGIAdapter::DebugCubeAdapter::SetWorldTM(const HDMath::HDFLOAT4X4& worldTM)
{
	DirectX::XMMATRIX tm;

	tm.r[0].m128_f32[0] = worldTM._11;
	tm.r[0].m128_f32[1] = worldTM._12;
	tm.r[0].m128_f32[2] = worldTM._13;
	tm.r[0].m128_f32[3] = worldTM._14;

	tm.r[1].m128_f32[0] = worldTM._21;
	tm.r[1].m128_f32[1] = worldTM._22;
	tm.r[1].m128_f32[2] = worldTM._23;
	tm.r[1].m128_f32[3] = worldTM._24;

	tm.r[2].m128_f32[0] = worldTM._31;
	tm.r[2].m128_f32[1] = worldTM._32;
	tm.r[2].m128_f32[2] = worldTM._33;
	tm.r[2].m128_f32[3] = worldTM._34;

	tm.r[3].m128_f32[0] = worldTM._41;
	tm.r[3].m128_f32[1] = worldTM._42;
	tm.r[3].m128_f32[2] = worldTM._43;
	tm.r[3].m128_f32[3] = worldTM._44;

	_debugCube->SetWorldTM(tm);
}

void hodoGIAdapter::DebugCubeAdapter::SetActive(bool isActive)
{
	_debugCube->SetActive(isActive);
}

void hodoGIAdapter::DebugCubeAdapter::SetFillModeSolid()
{
	_debugCube->SetFillModeSolid();
}

void hodoGIAdapter::DebugCubeAdapter::SetFillModeWireframe()
{
	_debugCube->SetFillModeWireframe();
}

void hodoGIAdapter::DebugCubeAdapter::SetColor(const HDMath::HDFLOAT4& color)
{
	_debugCube->SetColor(DirectX::XMFLOAT4{ color.r, color.g, color.b, color.a });
}
