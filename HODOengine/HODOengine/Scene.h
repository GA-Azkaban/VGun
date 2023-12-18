#pragma once
#include <string>
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
		HDData::GameObject* CreateObject(std::string objectName = "", HDData::GameObject* parent = nullptr);
		void DestroyObject(HDData::GameObject* gameObject);

		void FlushDestroyObjectList();

		void Start();
		void Update();
		void LateUpdate();
		void FixedUpdate();

		std::vector<GameObject*>& GetGameObjectList();
		std::vector<GameObject*>& GetDestroyObjectList();
		std::string GetSceneName();
		void SetSceneName(std::string sceneName);

	private:
		std::vector<GameObject*> _gameObjects;
		std::vector<GameObject*> _destroyObjects;
		std::string _sceneName;
		
	public:
		Camera* GetMainCamera();
		void SetMainCamera(Camera* camera);

	private:
		Camera* _mainCamera;
	};
}

