#pragma once
#include <unordered_map>

#include "Singleton.h"
#include "Scene.h" 

namespace hodoEngine
{
	class hodoData::Scene;
	class hodoData::GameObject;

	class SceneSystem : public Singleton<SceneSystem>
	{
		friend Singleton;

	private:
		SceneSystem() = default;

	public:
		void Initialize();
		void Update();

	public:
		hodoData::Scene* CreateScene();
		hodoData::Scene* CreateScene(std::string sceneName);
		bool LoadScene(std::string id);
		
	private:
		std::unordered_map<std::string, hodoData::Scene*> _sceneList;

	public:
		void SetCurrentSceneByName(std::string sceneName);
		hodoData::Scene* GetCurrentScene();
		bool GetIsCurrentSceneChange();

	private:
		hodoData::Scene* _currentScene;
		hodoData::Scene* _prevScene;
		bool _isSceneChange;
	};

}

