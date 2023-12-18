#include <cassert>

#include "Scene.h"
#include "GameObject.h"
#include "Camera.h"

#include "ObjectSystem.h"

namespace HDData
{
	Scene::Scene(std::string sceneName)
		: _sceneName(sceneName)
	{
		// ���� ������ �� �ݵ�� Directional Light�� Camera�� ������ �־�� �Ѵ�
		GameObject* camObj = HDEngine::ObjectSystem::Instance().CreateObject(this, "MainCamera");
		Camera* mainCam = camObj->AddComponent<Camera>();
		SetMainCamera(mainCam);
		camObj->GetTransform()->SetWorldPosition(0.0f, 2.0f, -10.0f);
	}

	Scene::~Scene()
	{
		
	}

	std::unordered_set<GameObject*>& Scene::GetGameObjectList()
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
		_mainCamera->SetAsMainCamera();
	}
}
