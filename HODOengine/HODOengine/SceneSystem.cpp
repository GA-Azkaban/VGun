#include "SceneSystem.h"
#include "Scene.h"

#include <cassert>

#include "IDSystem.h"

namespace HDEngine
{

	SceneSystem::SceneSystem()
		:_currentScene(nullptr)
	{

	}

	HDData::Scene* SceneSystem::CreateScene(std::string sceneName)
	{
		HDData::Scene* scene = new HDData::Scene(sceneName);
		_sceneList.insert({ sceneName, scene });
		return scene;
	}

	void SceneSystem::LoadScene(std::string sceneName)
	{
		auto sceneIter = _sceneList.find(sceneName);
		if (sceneIter == _sceneList.end())
		{
			return;
		}
		_currentScene = sceneIter->second;
	}

	void SceneSystem::LoadScene(HDData::Scene* scene)
	{
		_currentScene = scene;
	}

	std::unordered_map<std::string, HDData::Scene*>& SceneSystem::GetAllScenes()
	{
		return _sceneList;
	}

	HDData::Scene* SceneSystem::GetCurrentScene()
	{
		return _currentScene;
	}

}
