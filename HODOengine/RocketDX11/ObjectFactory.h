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
		virtual HDEngine::ICubeMap* GetCubeMap() override;
		virtual HDEngine::ISketchableText* CreateText() override;
		virtual HDEngine::ISketchableImage* CreateImage() override;
		virtual HDEngine::ILight* CreateLight() override;
		virtual HDEngine::ILineRenderer* CreateLineRenderer() override;
		virtual HDEngine::IMaterial* CreateMaterial(const HDEngine::MaterialDesc& desc) override;
		virtual HDEngine::IMaterial* GetMaterial(const std::string& name) override;
		virtual HDEngine::IParticleSystem* CreateParticleSystem() override;
		virtual HDEngine::CubePrimitive* CreateCubePrimitive() override;
		virtual HDEngine::SpherePrimitive* CreateSpherePrimitive() override;
		virtual HDEngine::CylinderPrimitive* CreateCylinderPrimitive() override;
		virtual HDEngine::CapsulePrimitive* CreateCapsulePrimitive() override;
	};
}

