#pragma once

/// <summary>
/// 오수안
/// 일단 임시로 하나의 API 클래스로 통합해둔다.
/// 추후에 기능별로 진입해서? 사용할 수 있도록 예쁘게 래핑할 예정
/// </summary>

#include "DLLExporter.h"

#include <windows.h>
#include <string>
#include "..\HODOmath\HODOmath.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\HODOmath.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\HODOmath.lib")
#endif // _DEBUG

#include "Scene.h"
#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "Camera.h"
#include "Script.h"
#include "Collider.h"
#include "StaticBoxCollider.h"
#include "DynamicBoxCollider.h"
#include "DynamicCapsuleCollider.h"
#include "DynamicSphereCollider.h"
#include "MeshRenderer.h"

#include "ObjectSystem.h"

namespace API
{
	extern "C"
	{
		/// 씬을 생성하기 위한 함수
		HODO_API HDData::Scene* CreateScene(std::string sceneName);
		HODO_API void LoadScene(HDData::Scene* scene);
		HODO_API HDData::GameObject* CreateObject(HDData::Scene* scene, std::string objectName = "", HDData::GameObject* parentObject = nullptr);
		
		HODO_API bool GetKeyDown(int keyCode);
		HODO_API bool GetKeyUp(int keyCode);
		HODO_API bool GetKeyPressing(int keyCode);
		HODO_API HDMath::HDFLOAT2 GetMousePosition();
		HODO_API HDMath::HDFLOAT2 GetMousePositionNormalized();

		HODO_API void DebugModeOn(int flag);

		HODO_API float GetDeltaTime();
	}
}

