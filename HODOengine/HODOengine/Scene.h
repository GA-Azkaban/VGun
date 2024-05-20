#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#include "dllExporter.h"

namespace HDData
{
	class GameObject;
	class Camera;
	class Light;

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

		void Update();
		void LateUpdate();
		void FixedUpdate();

		std::vector<GameObject*>& GetGameObjectList();
		std::vector<GameObject*>& GetRunningObjectList();
		std::vector<GameObject*>& GetDestroyObjectList();
		std::string GetSceneName();
		void SetSceneName(std::string sceneName);

	private:
		std::vector<GameObject*> _gameObjects;
		std::vector<GameObject*> _runningObjects;
		std::vector<GameObject*> _destroyObjects;
		std::string _sceneName;
		
	public:
		Camera* GetMainCamera();
		void SetMainCamera(Camera* camera);
		Light* GetMainLight();
		void SetMainLight(Light* light);

	private:
		Camera* _mainCamera;
		Light* _mainLight;
	};
}

