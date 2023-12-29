#pragma once
#include "MZDX11Renderer.h"
#include "RasterizerState.h"
#include "../HODO3DGraphicsInterface/IFactory.h"
#include "Hodo3DCameraAdapter.h"
#include "DebugCube.h"
#include "HodoDebugCubeAdapter.h"

namespace hodoGIAdapter
{
	class FactoryAdapter : public HDEngine::IFactory
	{
	public:
		virtual HDEngine::ICamera* CreateCamera() override
		{
			return new hodoGIAdapter::CameraAdapter();
		}

		virtual HDEngine::IStaticMesh* CreateStaticMeshObject() override
		{
			return nullptr;
		}
	};
}
