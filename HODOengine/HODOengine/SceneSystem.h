#pragma once
#include <unordered_map>
#include <string>
#include "Singleton.h"

namespace HDData
{
	class Scene;
}

namespace HDEngine
{
	class SceneSystem : public Singleton<SceneSystem>
	{
		friend Singleton;

	private:
		SceneSystem();

	public:
		HDData::Scene* CreateScene(std::string sceneName = "");
		void LoadScene(std::string sceneName);
		void LoadScene(HDData::Scene* scene);

		std::unordered_map<std::string, HDData::Scene*>& GetAllScenes();
		HDData::Scene* GetCurrentScene();

	private:
		std::unordered_map<std::string, HDData::Scene*> _sceneList;
		HDData::Scene* _currentScene;
	};

}

