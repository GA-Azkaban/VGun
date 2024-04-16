#include "SceneSystem.h"
#include "UISystem.h"
#include "GameObject.h"
#include "Scene.h"

#include <cassert>

#include "IDSystem.h"

namespace HDEngine
{

	SceneSystem::SceneSystem()
		:_currentScene(nullptr)
	{

	}

	void SceneSystem::UpdateNextScene()
	{
		if (_nextScene == nullptr) return;

		_currentScene = _nextScene;
	}

	HDData::Scene* SceneSystem::CreateScene(std::string sceneName)
	{
		auto iter = _sceneList.find(sceneName);
		if (iter != _sceneList.end())
		{
			return _sceneList[sceneName];
		}

		HDData::Scene* scene = new HDData::Scene(sceneName);
		_sceneList.insert({ sceneName, scene });

		if (!_currentScene)
		{
			_currentScene = scene;
		}

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

		UISystem::Instance().SetChangedScene(_currentScene);
	}

	void SceneSystem::LoadScene(HDData::Scene* scene)
	{
		_currentScene = scene;
		UISystem::Instance().SetChangedScene(_currentScene);
	}

	void SceneSystem::LoadNextScene(std::string sceneName)
	{
		auto sceneIter = _sceneList.find(sceneName);
		if (sceneIter == _sceneList.end())
		{
			return;
		}
		_nextScene = sceneIter->second;
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
