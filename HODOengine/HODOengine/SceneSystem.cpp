#include "SceneSystem.h"
#include "Scene.h"

#include <cassert>

#include "IDSystem.h"

namespace hodoEngine
{

	SceneSystem::SceneSystem()
		:_currentScene(nullptr)
	{

	}

	hodoEngine::Scene* SceneSystem::CreateScene(std::string sceneName)
	{
		Scene* scene = new Scene(sceneName);
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

	void SceneSystem::LoadScene(Scene* scene)
	{
		_currentScene = scene;
	}

	hodoEngine::Scene* SceneSystem::GetCurrentScene()
	{
		return _currentScene;
	}

}
