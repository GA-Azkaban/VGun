#include "Scene.h"
#include "GameObject.h"

namespace hodoEngine
{


	Scene::Scene(std::string sceneName)
	{
		// ���� ������ �� �ݵ�� Directional Light�� Camera�� ������ �־�� �Ѵ�
	}

	Scene::~Scene()
	{

	}

	void Scene::Start()
	{

	}

	void Scene::Update()
	{

	}

	hodoEngine::GameObject* Scene::CreateObject(std::string objName, GameObject* parentObject = nullptr)
	{
		GameObject* one = new GameObject(objName);
		_objList.push_back(one);
		if (parentObject != nullptr)
		{
			one->SetParentObject(parentObject);
		}
		return one;
	}

	bool Scene::DeleteObject()
	{
		return {};
	}

	std::vector<GameObject*>& Scene::GetObjList()
	{
		return _objList;
	}

	std::vector<GameObject*>& Scene::GetActiveObjList()
	{
		return _activeObjList;
	}

	std::string Scene::GetSceneName()
	{
		return _scneneName;
	}

	void Scene::SetSceneName(std::string sceneName)
	{
		_scneneName = sceneName;
	}

}
