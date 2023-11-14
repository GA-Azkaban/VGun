#pragma once

/// <summary>
/// 오수안
/// 일단 임시로 하나의 API 클래스로 통합해둔다.
/// 추후에 기능별로 진입해서? 사용할 수 있도록 예쁘게 래핑할 예정
/// </summary>

#include "DLLExporter.h"

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

#include "SceneSystem.h"

namespace hodoEngine
{
	/// 씬을 생성하기 위한 함수
	HODO_API Scene* CreateScene(std::string sceneName);
	HODO_API Scene* GetCurrentScene();
}

