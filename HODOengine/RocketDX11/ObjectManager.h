#pragma once
#include <vector>

#include "Singleton.h"
#include "../HODO3DGraphicsInterface/IMaterial.h"
#include "../HODO3DGraphicsInterface/IParticle.h"


namespace HDEngine
{
	struct CubePrimitive;
	struct SpherePrimitive;
	struct CylinderPrimitive;
	struct CapsulePrimitive;
}

namespace RocketCore::Graphics
{
	class Camera;
	class HelperObject;
	class StaticMeshObject;
	class SkinningMeshObject;
	class Cubemap;
	class LightAdapter;
	class TextRenderer;
	class ImageRenderer;
	class LineRenderer;
	class Material;
	

	class ObjectManager : public Singleton<ObjectManager>
	{
		friend Singleton;
	private:
		ObjectManager();

	public:
		std::vector<HelperObject*>& GetHelperObjList();
		std::vector<StaticMeshObject*>& GetStaticMeshObjList();
		std::vector<SkinningMeshObject*>& GetSkinningMeshObjList();
		std::vector<TextRenderer*>& GetTextList();
		std::vector<ImageRenderer*>& GetImageList();
		LineRenderer* GetLineRenderer();
		std::vector<HDEngine::CubePrimitive*>& GetCubePrimitiveList() { return _cubePrimitiveList; }
		std::vector<HDEngine::SpherePrimitive*>& GetSpherePrimitiveList() { return _spherePrimitiveList; }
		std::vector<HDEngine::CylinderPrimitive*>& GetCylinderPrimitiveList() { return _cylinderPrimitiveList; }
		std::vector<HDEngine::CapsulePrimitive*>& GetCapsulePrimitiveList() { return _capsulePrimitiveList; }

	public:
		Camera* CreateCamera();
		HelperObject* CreateHelperObject();
		StaticMeshObject* CreateStaticMeshObject();
		SkinningMeshObject* CreateSkinningMeshObject();
		Cubemap* CreateCubeMap();
		LightAdapter* CreateLight();
		TextRenderer* CreateText();
		ImageRenderer* CreateImage();
		LineRenderer* CreateLineRenderer();
		Material* CreateMaterial(HDEngine::MaterialDesc desc);
		HDEngine::IParticle* CreateParticle();

		HDEngine::CubePrimitive* CreateCubePrimitive();
		HDEngine::SpherePrimitive* CreateSpherePrimitive();
		HDEngine::CylinderPrimitive* CreateCylinderPrimitive();
		HDEngine::CapsulePrimitive* CreateCapsulePrimitive();

	private:
		std::vector<Camera*> _cameraList;
		std::vector<HelperObject*> _helperObjectList;
		std::vector<StaticMeshObject*> _staticMeshObjectList;
		std::vector<SkinningMeshObject*> _skinningMeshObjectList;
		std::vector<TextRenderer*> _textList;
		std::vector<ImageRenderer*> _ImageList;
		LineRenderer* _lineRenderer;
		std::vector<HDEngine::CubePrimitive*> _cubePrimitiveList;
		std::vector<HDEngine::SpherePrimitive*> _spherePrimitiveList;
		std::vector<HDEngine::CylinderPrimitive*> _cylinderPrimitiveList;
		std::vector<HDEngine::CapsulePrimitive*> _capsulePrimitiveList;
	};
}
