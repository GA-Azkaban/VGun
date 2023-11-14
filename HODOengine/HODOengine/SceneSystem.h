#pragma once
#include <unordered_map>

#include "Singleton.h"
#include "Scene.h" 

namespace hodoEngine
{
	class Scene;
	class GameObject;

	using ID = std::string;

	class SceneSystem : public Singleton<SceneSystem>
	{
		friend Singleton;

	private:
		SceneSystem() = default;

	public:
		void Initialize();
		void Update();

	public:
		Scene* CreateScene();
		Scene* CreateScene(std::string sceneName);
		bool LoadScene(ID id);
		
	private:
		std::unordered_map<ID, Scene*> _sceneList;

	public:
		void SetCurrentSceneByID(ID sceneID);
		void SetCurrentSceneByName(std::string sceneName);
		Scene* GetCurrentScene();
		bool GetIsCurrentSceneChange();

	private:
		Scene* _currentScene;
		Scene* _prevScene;
		bool _isSceneChange;
	};

}

