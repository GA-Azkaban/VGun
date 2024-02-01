#pragma once
#include "..\\HODO3DGraphicsInterface\\IFactory.h"

namespace RocketCore::Graphics
{
	class ObjectFactory : public HDEngine::IFactory
	{
	public:
		virtual HDEngine::ICamera* CreateCamera() override;
		virtual HDEngine::IStaticMesh* CreateStaticMeshObject() override;
		virtual HDEngine::ISkinnedMesh* CreateSkinnedMeshObject() override;
		virtual HDEngine::ICubeMap* CreateCubeMap() override;
		virtual HDEngine::ISketchableText* CreateText() override;
		virtual HDEngine::ISketchableImage* CreateImage() override;
		virtual HDEngine::ILineRenderer* CreateLineRenderer() override;
		virtual HDEngine::CubePrimitive* CreateCubePrimitive() override;
		virtual HDEngine::SpherePrimitive* CreateSpherePrimitive() override;
		virtual HDEngine::CylinderPrimitive* CreateCylinderPrimitive() override;
	};
}

