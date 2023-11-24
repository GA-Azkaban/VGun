#pragma once
#include <string>
#include <unordered_set>
#include <vector>

#include "dllExporter.h"

namespace hodoData
{
	class GameObject;

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
	};
}

