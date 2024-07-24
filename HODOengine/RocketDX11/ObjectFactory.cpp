#include "ObjectFactory.h"
#include "ObjectManager.h"
#include "ResourceManager.h"

#include "Camera.h"
#include "StaticMeshObject.h"
#include "SkinningMeshObject.h"
#include "Cubemap.h"
#include "LightAdapter.h"
#include "CubeMesh.h"
#include "TextRenderer.h"
#include "ImageRenderer.h"
#include "LineRenderer.h"
#include "Material.h"
#include <unordered_map>
#include <string>

namespace HDEngine
{
	IFactory* CreateGraphicsObjectFactory()
	{
		return new RocketCore::Graphics::ObjectFactory();
	}

	void ReleaseFactory(IFactory* instance)
	{
		delete instance;
	}
}

namespace RocketCore::Graphics
{

	HDEngine::ICamera* ObjectFactory::CreateCamera()
	{
		return ObjectManager::Instance().CreateCamera();
	}

	HDEngine::IStaticMesh* ObjectFactory::CreateStaticMeshObject()
	{
		return ObjectManager::Instance().CreateStaticMeshObject();
	}

	HDEngine::ISkinnedMesh* ObjectFactory::CreateSkinnedMeshObject()
	{
		return ObjectManager::Instance().CreateSkinningMeshObject();
	}

	HDEngine::ICubeMap* ObjectFactory::GetCubeMap()
	{
		return ObjectManager::Instance().GetCubeMap();
	}

	HDEngine::ISketchableText* ObjectFactory::CreateText()
	{
		return ObjectManager::Instance().CreateText();
	}

	HDEngine::ISketchableImage* ObjectFactory::CreateImage()
	{
		return ObjectManager::Instance().CreateImage();
	}

	HDEngine::ILight* ObjectFactory::CreateLight()
	{
		return ObjectManager::Instance().CreateLight();
	}

	HDEngine::ILineRenderer* ObjectFactory::CreateLineRenderer()
	{
		return ObjectManager::Instance().CreateLineRenderer();
	}

	HDEngine::IMaterial* ObjectFactory::CreateMaterial(const HDEngine::MaterialDesc& desc)
	{				
		return ObjectManager::Instance().CreateMaterial(desc);
	}

	HDEngine::IMaterial* ObjectFactory::GetMaterial(const std::string& name)
	{
		return ResourceManager::Instance().GetLoadedMaterial(name);
	}

	HDEngine::IParticleSystem* ObjectFactory::CreateParticleSystem()
	{
		return ObjectManager::Instance().CreateParticleSystem();
	}

	HDEngine::CubePrimitive* ObjectFactory::CreateCubePrimitive()
	{
		return ObjectManager::Instance().CreateCubePrimitive();
	}

	HDEngine::SpherePrimitive* ObjectFactory::CreateSpherePrimitive()
	{
		return ObjectManager::Instance().CreateSpherePrimitive();
	}

	HDEngine::CylinderPrimitive* ObjectFactory::CreateCylinderPrimitive()
	{
		return ObjectManager::Instance().CreateCylinderPrimitive();
	}

	HDEngine::CapsulePrimitive* ObjectFactory::CreateCapsulePrimitive()
	{
		return ObjectManager::Instance().CreateCapsulePrimitive();
	}

}
