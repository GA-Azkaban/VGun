#include <cassert>

#include "Scene.h"
#include "GameObject.h"
#include "Camera.h"
#include "AudioListener.h"

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
		camObj->GetTransform()->SetPosition(0.0f, 2.0f, -10.0f);
		// 씬이 생성될 때 메인카메라에 오디오리스너 컴포넌트를 생성하여 부착한다
		camObj->AddComponent<AudioListner>();
	}

	Scene::~Scene()
	{
		
	}

	HDData::GameObject* Scene::CreateObject(std::string objectName /*= ""*/, HDData::GameObject* parent /*= nullptr*/)
	{
		auto obj = HDEngine::ObjectSystem::Instance().CreateObject(this, objectName, parent);

		return obj;
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
				GetRunningObjectList().push_back(gameObject);
			}
			_gameObjects.clear();
		}

		for (auto& gameObject : GetRunningObjectList())
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

	std::vector<GameObject*>& Scene::GetRunningObjectList()
	{
		return _runningObjects;
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
		_mainCamera->SetAsMainCamera();
	}
}
