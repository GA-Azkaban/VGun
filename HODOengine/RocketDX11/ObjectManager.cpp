#include "ObjectManager.h"

#include "..\\HODO3DGraphicsInterface\\IRenderable.h"
#include "../HODO3DGraphicsInterface/PrimitiveHeader.h"

#include "Camera.h"
#include "HelperObject.h"
#include "StaticMeshObject.h"
#include "SkinningMeshObject.h"
#include "Cubemap.h"
#include "LightAdapter.h"
#include "TextRenderer.h"
#include "ImageRenderer.h"
#include "ResourceManager.h"
#include "LineRenderer.h"
#include "Material.h"

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

	SkinningMeshObject* ObjectManager::CreateSkinningMeshObject()
	{
		SkinningMeshObject* temp = new SkinningMeshObject();
		_skinningMeshObjectList.emplace_back(temp);

		return temp;
	}

	Cubemap* ObjectManager::CreateCubeMap()
	{
		Cubemap* temp = &(Cubemap::Instance());
		return temp;
	}

	LightAdapter* ObjectManager::CreateLight()
	{
		LightAdapter* temp = new LightAdapter();
		return temp;
	}

	ImageRenderer* ObjectManager::CreateImage()
	{
		auto& resourceMgr = ResourceManager::Instance();

		ImageRenderer* temp = new ImageRenderer();
		temp->InitalizeImageRenderer(resourceMgr.GetDevice(), resourceMgr.GetDeviceContext());
		temp->SetImage("defaultImg.png");
		_ImageList.emplace_back(temp);

		return temp;
	}

	LineRenderer* ObjectManager::CreateLineRenderer()
	{
		_lineRenderer = new LineRenderer();
		return _lineRenderer;
	}

	RocketCore::Graphics::Material* ObjectManager::CreateMaterial(HDEngine::MaterialDesc desc)
	{
		std::unordered_map<std::string, Material*>& materialList = ResourceManager::Instance().GetLoadedMaterials();

		std::string matName = desc.materialName;
		if (matName == "")
		{
			matName = "NewMaterial";
		}

		// 이미 있는 이름이라면 뒤에 숫자를 붙혀준다.
		if (materialList.find(matName) != materialList.end())
		{
			std::string tempName = matName;
			for (UINT i = 1; ++i;)
			{
				matName = tempName + std::to_string(i);
				if (materialList.find(matName) == materialList.end())
				{
					break;
				}
			}
		}
		desc.materialName = matName;

		Material* newMaterial = new Material(desc);
		materialList.insert(std::make_pair(matName, newMaterial));

		return newMaterial;
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

	HDEngine::CapsulePrimitive* ObjectManager::CreateCapsulePrimitive()
	{
		HDEngine::CapsulePrimitive* capsule = new HDEngine::CapsulePrimitive();
		capsule->isWire = true;
		_capsulePrimitiveList.emplace_back(capsule);
		return capsule;
	}

	TextRenderer* ObjectManager::CreateText()
	{
		TextRenderer* TextObject = new TextRenderer();
		_textList.emplace_back(TextObject);
		return TextObject;
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

	std::vector<TextRenderer*>& ObjectManager::GetTextList()
	{
		return _textList;
	}

	std::vector<ImageRenderer*>& ObjectManager::GetImageList()
	{
		return _ImageList;
	}

	LineRenderer* ObjectManager::GetLineRenderer()
	{
		return _lineRenderer;
	}

}
