#pragma once
#include <unordered_map>
#include <string>
#include "Singleton.h"

namespace HDData
{
	class Scene;
	class GameObject;
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
		void UpdateScene();
		
	private:
		std::unordered_map<std::string, HDData::Scene*> _sceneList;

	public:
		HDData::Scene* GetCurrentScene();

	private:
		HDData::Scene* _currentScene;
	};

}

