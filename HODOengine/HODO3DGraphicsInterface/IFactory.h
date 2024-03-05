#pragma once

/// <summary>
/// 팩토리 객체는 카메라, 메쉬, 라이트와 같은 렌더링에 필요한 객체들을 생성한다.
/// 2023.11.10 김민정
/// </summary>

namespace HDEngine
{
	class ICamera;
	class IStaticMesh;
	class ISkinnedMesh;
	class ICubeMap;
	class ISketchableText;
	class ISketchableImage;
	class ILight;
	class ILineRenderer;
	struct CubePrimitive;
	struct SpherePrimitive;
	struct CylinderPrimitive;
	struct CapsulePrimitive;

	class IFactory
	{
	public:
		virtual ICamera* CreateCamera() = 0;
		virtual IStaticMesh* CreateStaticMeshObject() = 0;
		virtual ISkinnedMesh* CreateSkinnedMeshObject() = 0;
		virtual ICubeMap* CreateCubeMap() = 0;
		virtual ISketchableText* CreateText() = 0;
		virtual ISketchableImage* CreateImage() = 0;
		virtual ILight* CreateLight() = 0;
		virtual ILineRenderer* CreateLineRenderer() = 0;
		virtual CubePrimitive* CreateCubePrimitive() = 0;
		virtual SpherePrimitive* CreateSpherePrimitive() = 0;
		virtual CylinderPrimitive* CreateCylinderPrimitive() = 0;
		virtual CapsulePrimitive* CreateCapsulePrimitive() = 0;
	};

	/// dllexport 하는 오브젝트팩토리를 반환하는 팩토리 함수.

	extern "C" __declspec(dllexport) IFactory* CreateGraphicsObjectFactory();
	extern "C" __declspec(dllexport) void ReleaseFactory(IFactory* instance);
}
