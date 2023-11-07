#pragma once
#include <unordered_map>

#include "Singleton.h"
#include "Scene.h" 

namespace hodoEngine
{
	class Scene;

	class SceneSystem
	{

	public:
		Scene* CreateScene(std::string sceneName);

	private:
		// ���̵� ���� �ϵ��� �����ϰ� ����
		std::unordered_map<std::string, Scene*> _sceneList;
	};

}

