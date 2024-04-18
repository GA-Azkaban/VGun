#include "HodoDebugCubeAdapter.h"

void hodoGIAdapter::DebugCubeAdapter::SetWorldTM(const Matrix& worldTM)
{
	_debugCube->SetWorldTM(worldTM);
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

void hodoGIAdapter::DebugCubeAdapter::SetColor(const Vector4& color)
{
	_debugCube->SetColor(color);
}
