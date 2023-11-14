#pragma once

/// <summary>
/// ������
/// �ϴ� �ӽ÷� �ϳ��� API Ŭ������ �����صд�.
/// ���Ŀ� ��ɺ��� �����ؼ�? ����� �� �ֵ��� ���ڰ� ������ ����
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
	/// ���� �����ϱ� ���� �Լ�
	HODO_API Scene* CreateScene(std::string sceneName);
	HODO_API Scene* GetCurrentScene();
}

