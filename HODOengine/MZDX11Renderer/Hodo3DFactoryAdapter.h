#pragma once
#include "MZDX11Renderer.h"
#include "RasterizerState.h"
#include "../HODO3DGraphicsInterface/IFactory.h"
#include "Hodo3DCameraAdapter.h"
#include "DebugCube.h"
#include "HodoDebugCubeAdapter.h"
#include "StaticMeshObject.h"
#include "HodoStaticMeshAdapter.h"
#include "SkinningMeshObject.h"
#include "HodoSkinnedMeshAdapter.h"

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
			StaticMeshObject* staticMesh = new StaticMeshObject();
			return new hodoGIAdapter::StaticMeshAdapter(staticMesh);
		}
		
		virtual HDEngine::ISkinnedMesh* CreateSkinnedMeshObject() override
		{
			SkinningMeshObject* skinningMesh = new SkinningMeshObject();
			return new hodoGIAdapter::SkinnedMeshAdapter(skinningMesh);
		}
	};
}
