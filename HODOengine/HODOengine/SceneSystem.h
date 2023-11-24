#pragma once
#include <unordered_map>

#include "Singleton.h"

namespace hodoEngine
{
	class Scene;
	class GameObject;

	class SceneSystem : public Singleton<SceneSystem>
	{
		friend Singleton;

	private:
		SceneSystem();

	public:
		Scene* CreateScene(std::string sceneName = "");
		void LoadScene(std::string sceneName);
		void LoadScene(Scene* scene);
		
	private:
		std::unordered_map<std::string, Scene*> _sceneList;

	public:
		Scene* GetCurrentScene();
		bool GetIsCurrentSceneChange();

	private:
		Scene* _currentScene;
	};

}

