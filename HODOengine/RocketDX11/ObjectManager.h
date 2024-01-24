#pragma once
#include <vector>

#include "Singleton.h"


namespace HDEngine
{
	struct CubePrimitive;
	struct SpherePrimitive;
	struct CylinderPrimitive;
}

namespace RocketCore::Graphics
{
	class Camera;
	class HelperObject;
	class StaticMeshObject;
	class SkinningMeshObject;
	class TextRenderer;
	class ImageRenderer;
	class LineRenderer;

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

	public:
		Camera* CreateCamera();
		HelperObject* CreateHelperObject();
		StaticMeshObject* CreateStaticMeshObject();
		SkinningMeshObject* CreateSkinningMeshObject();
		TextRenderer* CreateText();
		ImageRenderer* CreateImage();
		LineRenderer* CreateLineRenderer();
		HDEngine::CubePrimitive* CreateCubePrimitive();
		HDEngine::SpherePrimitive* CreateSpherePrimitive();
		HDEngine::CylinderPrimitive* CreateCylinderPrimitive();

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
	};
}
