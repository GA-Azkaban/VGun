#pragma once
#include "MZDX11Renderer.h"
#include "RasterizerState.h"
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
			DebugCube* debugCube{ new DebugCube(MZDX11Renderer::Instance().GetDevice(), MZDX11Renderer::Instance().GetDeviceContext(), RasterizerState::Instance.Get().GetWireframeRS()) };
			return new hodoGIAdapter::DebugCubeAdapter(debugCube);
		}
	};
}