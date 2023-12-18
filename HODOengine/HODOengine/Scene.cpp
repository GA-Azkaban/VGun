#include <cassert>

#include "Scene.h"
#include "GameObject.h"
#include "Camera.h"

#include "ObjectSystem.h"
#include <algorithm>

namespace HDData
{
	Scene::Scene(std::string sceneName)
		: _sceneName(sceneName)
	{
		// 씬이 생성될 때 반드시 Directional Light와 Camera를 가지고 있어야 한다
		GameObject* camObj = CreateObject("MainCamera");
		Camera* mainCam = camObj->AddComponent<Camera>();
		SetMainCamera(mainCam);
		camObj->GetTransform()->SetWorldPosition(0.0f, 5.0f, 20.0f);
		camObj->GetTransform()->Rotate(0.0f, 180.0f, 0.0f);
	}

	Scene::~Scene()
	{
		
	}

	HDData::GameObject* Scene::CreateObject(std::string objectName /*= ""*/, HDData::GameObject* parent /*= nullptr*/)
	{
		return HDEngine::ObjectSystem::Instance().CreateObject(this, objectName, parent);
	}

	void Scene::DestroyObject(HDData::GameObject* gameObject)
	{
		HDEngine::ObjectSystem::Instance().DestroyObject(this, gameObject);
	}

	void Scene::FlushDestroyObjectList()
	{
		for (auto& destroyObj : _destroyObjects)
		{
			for (auto& component : destroyObj->GetAllComponents())
			{
				component->OnDestroy();
			}
			_gameObjects.erase(std::remove_if(_destroyObjects.begin(), _destroyObjects.end(), [&](HDData::GameObject* gameObject)->bool { return gameObject == destroyObj; }));
		}
		_destroyObjects.clear();
	}

	void Scene::Update()
	{
		if (!_gameObjects.empty())
		{
			for (auto& gameObject : _gameObjects)
			{
				if (gameObject->IsActive())
				{
					gameObject->Start();
				}
				_runningObjects.push_back(gameObject);
			}
			_gameObjects.clear();
		}

		for (auto& gameObject : _gameObjects)
		{
			if (gameObject->IsActive())
			{
				gameObject->Update();
			}
		}
	}

	void Scene::LateUpdate()
	{
		for (auto& gameObject : _gameObjects)
		{
			if (gameObject->IsActive())
			{
				gameObject->LateUpdate();
			}
		}
	}

	void Scene::FixedUpdate()
	{
		for (auto& gameObject : _gameObjects)
		{
			if (gameObject->IsActive())
			{
				gameObject->FixedUpdate();
			}
		}
	}

	std::vector<GameObject*>& Scene::GetGameObjectList()
	{
		return _gameObjects;
	}

	std::vector<GameObject*>& Scene::GetDestroyObjectList()
	{
		return _destroyObjects;
	}

	std::string Scene::GetSceneName()
	{
		return _sceneName;
	}

	void Scene::SetSceneName(std::string sceneName)
	{
		_sceneName = sceneName;
	}

	HDData::Camera* Scene::GetMainCamera()
	{
		assert(_mainCamera);
		return _mainCamera;
	}

	void Scene::SetMainCamera(Camera* camera)
	{
		_mainCamera = camera;
	}
}
