#include "Scene.h"
#include "GameObject.h"

namespace hodoData
{
	Scene::Scene(std::string sceneName)
		: _sceneName(sceneName)
	{
		// ���� ������ �� �ݵ�� Directional Light�� Camera�� ������ �־�� �Ѵ�
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

}
