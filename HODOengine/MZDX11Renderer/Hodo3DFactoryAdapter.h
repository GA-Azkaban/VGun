#pragma once
#include "DeferredRenderer.h"
#include "IHodoGIFactory.h"
#include "Hodo3DCameraAdapter.h"
#include "DebugCube.h"
#include "HodoDebugCubeAdapter.h"

namespace hodoGIAdapter
{
	class FactoryAdapter : public hodoGI::IFactory
	{
	public:
		virtual hodoGI::ICamera* CreateCamera() override
		{
			return new hodoGIAdapter::CameraAdapter();
		}

		virtual hodoGI::IDebugCube* CreateDebugCube() override
		{
			DebugCube* debugCube{ new DebugCube(DeferredRenderer::Instance.Get().GetDevice(), DeferredRenderer::Instance.Get().GetDeviceContext(), RasterizerState::WireframeRS.Get()) };
			return new hodoGIAdapter::DebugCubeAdapter(debugCube);
		}
	};
}