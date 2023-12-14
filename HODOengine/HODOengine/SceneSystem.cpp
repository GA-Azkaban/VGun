#include "SceneSystem.h"
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

	void SceneSystem::UpdateScene()
	{
		for (auto destroyObj : HDEngine::SceneSystem::Instance().GetCurrentScene()->GetDestroyObjectList())
		{
			for (auto component : destroyObj->GetAllComponents())
			{
				component->OnDestroy();
			}
			HDEngine::SceneSystem::Instance().GetCurrentScene()->GetGameObjectList().erase(destroyObj);
		}
		HDEngine::SceneSystem::Instance().GetCurrentScene()->GetDestroyObjectList().clear();

		for (const auto& one : _currentScene->GetGameObjectList())
		{
			for (const auto& comp : one->GetAllComponents())
			{
				comp->Update();
			}
		}
	}

	HDData::Scene* SceneSystem::GetCurrentScene()
	{
		return _currentScene;
	}

}
