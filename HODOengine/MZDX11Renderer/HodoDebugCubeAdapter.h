#pragma once
#include "IDebugCube.h"
#include "DebugCube.h"
#include "MathHeader.h"
#include <memory>

namespace hodoGIAdapter
{
	class DebugCubeAdapter : public HDEngine::IDebugCube
	{
	public:
		DebugCubeAdapter(DebugCube* debugCube)
			: _debugCube(debugCube)
		{

		}
		virtual void SetWorldTM(const Matrix& worldTM) override;
		virtual void SetActive(bool isActive) override;
		virtual void SetFillModeSolid() override;
		virtual void SetFillModeWireframe() override;
		virtual void SetColor(const Vector4& color) override;

	private:
		std::unique_ptr<DebugCube> _debugCube;
	};
}