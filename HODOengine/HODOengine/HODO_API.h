#pragma once

/// <summary>
/// ������
/// �ϴ� �ӽ÷� �ϳ��� API Ŭ������ �����صд�.
/// ���Ŀ� ��ɺ��� �����ؼ�? ����� �� �ֵ��� ���ڰ� ������ ����
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

#include "ObjectSystem.h"

namespace API
{
	extern "C"
	{
		/// ���� �����ϱ� ���� �Լ�
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

