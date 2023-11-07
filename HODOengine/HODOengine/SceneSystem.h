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
		// 아이디를 갖게 하도록 수정하고 싶음
		std::unordered_map<std::string, Scene*> _sceneList;
	};

}

