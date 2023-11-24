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

	hodoData::Scene* SceneSystem::GetCurrentScene()
	{
		return _currentScene;
	}

}
