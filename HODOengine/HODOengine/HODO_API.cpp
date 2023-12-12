#include "HODO_API.h"

#include "SceneSystem.h"
#include "ObjectSystem.h"
#include "InputSystem.h"
#include "TimeSystem.h"
#include "GameObject.h"
#include "DebugSystem.h"

namespace API
{
	extern "C"
	{
		HODO_API hodoData::Scene* CreateScene(std::string sceneName)
		{
			return hodoEngine::SceneSystem::Instance().CreateScene(sceneName);
		}
		HODO_API void LoadScene(hodoData::Scene* scene)
		{
			hodoEngine::SceneSystem::Instance().LoadScene(scene);
		}
		HODO_API hodoData::GameObject* CreateObject(hodoData::Scene* scene, std::string objectName, hodoData::GameObject* parentObject)
		{
			return hodoEngine::ObjectSystem::Instance().CreateObject(scene, objectName, parentObject);
		}
		HODO_API bool GetKeyDown(int keyCode)
		{
			return hodoEngine::InputSystem::Instance().GetKeyDown(keyCode);
		}
		HODO_API bool GetKeyUp(int keyCode)
		{
			return hodoEngine::InputSystem::Instance().GetKeyUp(keyCode);
		}
		HODO_API bool GetKeyPressing(int keyCode)
		{
			return hodoEngine::InputSystem::Instance().GetKeyPressing(keyCode);
		}
		HODO_API HDMaths::HDFLOAT2 GetMousePosition()
		{
			return hodoEngine::InputSystem::Instance().GetMousePosition();
		}
		HODO_API HDMaths::HDFLOAT2 GetMousePositionNormalized()
		{
			return hodoEngine::InputSystem::Instance().GetMousePositionNormalized();
		}
		HODO_API float GetDeltaTime()
		{
			return hodoEngine::TimeSystem::Instance().GetDeltaTime();
		}
		HODO_API void DebugModeOn(int flag)
		{
			hodoEngine::DebugSystem::Instance().SetDebugOn(flag);
		}
	}


}
