#pragma once
#include <unordered_map>
#include <string>
#include "Singleton.h"

namespace hodoData
{
	class Scene;
	class GameObject;
}

namespace hodoEngine
{
	class SceneSystem : public Singleton<SceneSystem>
	{
		friend Singleton;

	private:
		SceneSystem();

	public:
		hodoData::Scene* CreateScene(std::string sceneName = "");
		void LoadScene(std::string sceneName);
		void LoadScene(hodoData::Scene* scene);
		
	private:
		std::unordered_map<std::string, hodoData::Scene*> _sceneList;

	public:
		hodoData::Scene* GetCurrentScene();
		bool GetIsCurrentSceneChange();

	private:
		hodoData::Scene* _currentScene;
	};

}

