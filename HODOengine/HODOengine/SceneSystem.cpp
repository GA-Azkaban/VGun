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

	hodoEngine::Scene* SceneSystem::CreateScene()
	{
		std::string id = IDSystem::Instance().CreateID();
		Scene* scene = new Scene();
		_sceneList.insert({ id, scene });
		return scene;
	}

	hodoEngine::Scene* SceneSystem::CreateScene(std::string sceneName)
	{
		Scene* scene = new Scene(sceneName);
		_sceneList.insert({ sceneName, scene });
		return scene;
	}

	bool SceneSystem::LoadScene(ID id)
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

	hodoEngine::Scene* SceneSystem::GetCurrentScene()
	{
		return _currentScene;
	}

	bool SceneSystem::GetIsCurrentSceneChange()
	{
		return _isSceneChange;
	}

}
