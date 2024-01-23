#include "ObjectManager.h"

#include "..\\HODO3DGraphicsInterface\\IRenderable.h"
#include "../HODO3DGraphicsInterface/PrimitiveHeader.h"

#include "Camera.h"
#include "HelperObject.h"
#include "StaticMeshObject.h"
#include "SkinningMeshObject.h"
#include "TextRenderer.h"
#include "ImageRenderer.h"
#include "ResourceManager.h"
#include "LineRenderer.h"

namespace RocketCore::Graphics
{
	ObjectManager::ObjectManager()
	{

	}

	Camera* ObjectManager::CreateCamera()
	{
		Camera* temp = new Camera();
		_cameraList.emplace_back(temp);

		return temp;
	}

	HelperObject* ObjectManager::CreateHelperObject()
	{
		HelperObject* temp = new HelperObject();
		_helperObjectList.emplace_back(temp);

		return temp;
	}

	StaticMeshObject* ObjectManager::CreateStaticMeshObject()
	{
		StaticMeshObject* temp = new StaticMeshObject();
		_staticMeshObjectList.emplace_back(temp);

		return temp;
	}

	RocketCore::Graphics::SkinningMeshObject* ObjectManager::CreateSkinningMeshObject()
	{
		SkinningMeshObject* temp = new SkinningMeshObject();
		_skinningMeshObjectList.emplace_back(temp);

		return temp;
	}

	RocketCore::Graphics::ImageRenderer* ObjectManager::CreateImage()
	{
		auto& resourceMgr = ResourceManager::Instance();

		ImageRenderer* temp = new ImageRenderer();
		temp->InitalizeImageRenderer(resourceMgr.GetDevice(), resourceMgr.GetDeviceContext());
		temp->SetImage("abcd.jpg");
		_ImageList.emplace_back(temp);

		return temp;
	}

	std::vector<ImageRenderer*>& ObjectManager::GetImageList()
	{
		return _ImageList;
	}

	std::vector<HelperObject*>& ObjectManager::GetHelperObjList()
	{
		return _helperObjectList;
	}

	std::vector<StaticMeshObject*>& ObjectManager::GetStaticMeshObjList()
	{
		return _staticMeshObjectList;
	}

	std::vector<SkinningMeshObject*>& ObjectManager::GetSkinningMeshObjList()
	{
		return _skinningMeshObjectList;
	}

	RocketCore::Graphics::TextRenderer* ObjectManager::CreateText()
	{
		TextRenderer* TextObject = new TextRenderer();
		_textList.emplace_back(TextObject);
		return TextObject;
	}

	std::vector<TextRenderer*>& ObjectManager::GetTextList()
	{
		return _textList;
	}

	LineRenderer* ObjectManager::CreateLineRenderer()
	{
		_lineRenderer = new LineRenderer();
		return _lineRenderer;
	}

	LineRenderer* ObjectManager::GetLineRenderer()
	{
		return _lineRenderer;
	}

	HDEngine::CubePrimitive* ObjectManager::CreateCubePrimitive()
	{
		HDEngine::CubePrimitive* cube = new HDEngine::CubePrimitive();
		cube->isWire = true;
		_cubePrimitiveList.emplace_back(cube);
		return cube;
	}

	HDEngine::SpherePrimitive* ObjectManager::CreateSpherePrimitive()
	{
		HDEngine::SpherePrimitive* sphere = new HDEngine::SpherePrimitive();
		sphere->isWire = true;
		_spherePrimitiveList.emplace_back(sphere);
		return sphere;
	}

	HDEngine::CylinderPrimitive* ObjectManager::CreateCylinderPrimitive()
	{
		HDEngine::CylinderPrimitive* cylinder = new HDEngine::CylinderPrimitive();
		cylinder->isWire = true;
		_cylinderPrimitiveList.emplace_back(cylinder);
		return cylinder;
	}

}
