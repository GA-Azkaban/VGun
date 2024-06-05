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
		HDData::Scene* LoadScene(std::string sceneName);
		void LoadScene(HDData::Scene* scene);

		std::unordered_map<std::string, HDData::Scene*>& GetAllScenes();
		HDData::Scene* GetCurrentScene();
		HDData::Scene* GetPrevScene();


	private:
		std::unordered_map<std::string, HDData::Scene*> _sceneList;
		HDData::Scene* _currentScene;
		HDData::Scene* _prevScene;
	};

}

