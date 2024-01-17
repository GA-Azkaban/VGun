#include "ObjectFactory.h"
#include "ObjectManager.h"
#include "ResourceManager.h"

#include "Camera.h"
#include "StaticMeshObject.h"
#include "SkinningMeshObject.h"
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

	HDEngine::ISketchableText* ObjectFactory::CreateText()
	{
		return ObjectManager::Instance().CreateText();
	}

	HDEngine::ISketchableImage* ObjectFactory::CreateImage()
	{
		return ObjectManager::Instance().CreateImage();
	}

	HDEngine::ILineRenderer* ObjectFactory::CreateLineRenderer()
	{
		return ObjectManager::Instance().CreateLineRenderer();
	}

}
