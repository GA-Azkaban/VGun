#pragma once
#include <string>
#include <unordered_set>
#include <vector>

#include "dllExporter.h"

namespace HDData
{
	class GameObject;
	class Camera;

	class HODO_API Scene
	{
	public:
		Scene(std::string sceneName = "");
		Scene(const Scene&) = delete;
		Scene(Scene&&) = delete;
		Scene& operator=(const Scene&) = delete;
		Scene& operator=(Scene&&) = delete;
		~Scene();

	public:
		std::unordered_set<GameObject*>& GetGameObjectList();
		std::vector<GameObject*>& GetDestroyObjectList();
		std::string GetSceneName();
		void SetSceneName(std::string sceneName);

	private:
		std::unordered_set<GameObject*> _gameObjects;
		std::vector<GameObject*> _destroyObjects;
		std::string _sceneName;
		
	public:
		Camera* GetMainCamera();
		void SetMainCamera(Camera* camera);

	private:
		Camera* _mainCamera;
	};
}

