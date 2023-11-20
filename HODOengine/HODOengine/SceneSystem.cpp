#include "SceneSystem.h"

#include <cassert>

#include "IDSystem.h"
#include "ObjectSystem.h"

namespace hodoEngine
{

	void SceneSystem::Initialize()
	{
		_prevScene = nullptr;
		_currentScene = nullptr;
	}

	void SceneSystem::Update()
	{
		if (_prevScene != _currentScene)
		{
			_isSceneChange = true;
		}
		else
		{
			_isSceneChange = false;
		}
	}

	hodoData::Scene* SceneSystem::CreateScene()
	{
		std::string id = IDSystem::Instance().CreateID();
		hodoData::Scene* scene = new hodoData::Scene();
		_sceneList.insert({ id, scene });
		return scene;
	}

	hodoData::Scene* SceneSystem::CreateScene(std::string sceneName)
	{
		hodoData::Scene* scene = new hodoData::Scene(sceneName);
		_sceneList.insert({ sceneName, scene });
		return scene;
	}

	bool SceneSystem::LoadScene(std::string id)
	{
		auto scene = _sceneList.find(id);
		assert(scene == _sceneList.end(), "Scene not found");
		_prevScene = _currentScene;
		_currentScene = scene->second;
		return true;
	}

	void SceneSystem::SetCurrentSceneByName(std::string sceneName)
	{
		for (const auto& scene : _sceneList)
		{
			if (scene.second->GetSceneName() == sceneName)
			{
				_currentScene = scene.second;
			}
		}
	}

	hodoData::Scene* SceneSystem::GetCurrentScene()
	{
		return _currentScene;
	}

	bool SceneSystem::GetIsCurrentSceneChange()
	{
		return _isSceneChange;
	}

}
