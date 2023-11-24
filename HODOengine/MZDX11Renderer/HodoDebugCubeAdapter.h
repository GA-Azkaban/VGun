#pragma once
#include "IDebugCube.h"
#include "DebugCube.h"
#include <memory>

namespace hodoGIAdapter
{
	class DebugCubeAdapter : public hodoGI::IDebugCube
	{
	public:
		DebugCubeAdapter(DebugCube* debugCube)
			: _debugCube(debugCube)
		{

		}
		virtual void SetWorldTM(const HDMaths::HDFLOAT4X4& worldTM) override;
		virtual void SetActive(bool isActive) override;
		virtual void SetFillModeSolid() override;
		virtual void SetFillModeWireframe() override;
		virtual void SetColor(const hodoGI::Color& color) override;

	private:
		std::unique_ptr<DebugCube> _debugCube;
	};
}