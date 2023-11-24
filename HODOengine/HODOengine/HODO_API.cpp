#include "HODO_API.h"

#include "SceneSystem.h"
#include "ObjectSystem.h"
#include "GameObject.h"

namespace hodoEngine
{
	extern "C"
	{
		HODO_API hodoData::Scene* CreateScene(std::string sceneName)
		{
			return SceneSystem::Instance().CreateScene(sceneName);
		}
		HODO_API void LoadScene(hodoData::Scene* scene)
		{
			SceneSystem::Instance().LoadScene(scene);
		}
		HODO_API hodoData::GameObject* CreateObject(hodoData::Scene* scene, std::string objectName, hodoData::GameObject* parentObject)
		{
			return ObjectSystem::Instance().CreateObject(scene, objectName, parentObject);
		}
	}
}
