#include "HODO_API.h"

#include "SceneSystem.h"
#include "ObjectSystem.h"

namespace hodoData
extern "C"
{
	namespace hodoEngine 
	{
		HODO_API Scene* hodoEngine::CreateScene(std::string sceneName)
		{
			return SceneSystem::Instance().CreateScene(sceneName);
		}
		HODO_API void LoadScene(Scene* scene)
		{
			SceneSystem::Instance().LoadScene(scene);
		}
	HODO_API GameObject* hodoEngine::CreateObject(Scene* scene, std::string objectName, GameObject* parentObject)
	{
		return ObjectSystem::Instance().CreateObject(scene, objectName, parentObject);
	}
	
}

