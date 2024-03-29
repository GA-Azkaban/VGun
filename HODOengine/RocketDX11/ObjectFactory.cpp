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

	HDEngine::IResourceManager* ObjectFactory::CreateResourceManager()
	{
		return &(ResourceManager::Instance());
	}

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

	HDEngine::ICubeMap* ObjectFactory::CreateCubeMap()
	{
		return ObjectManager::Instance().CreateCubeMap();
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
