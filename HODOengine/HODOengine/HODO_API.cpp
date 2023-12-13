#include "HODO_API.h"

#include "SceneSystem.h"
#include "ObjectSystem.h"
#include "InputSystem.h"
#include "TimeSystem.h"
#include "GameObject.h"
#include "DebugSystem.h"

namespace HDEngine
{
	extern "C"
	{
		HODO_API HDData::Scene* CreateScene(std::string sceneName)
		{
			return hodoEngine::SceneSystem::Instance().CreateScene(sceneName);
		}
		HODO_API void LoadScene(HDData::Scene* scene)
		{
			hodoEngine::SceneSystem::Instance().LoadScene(scene);
		}
		HODO_API HDData::GameObject* CreateObject(HDData::Scene* scene, std::string objectName, HDData::GameObject* parentObject)
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
		HODO_API HDMath::HDFLOAT2 GetMousePosition()
		{
			return hodoEngine::InputSystem::Instance().GetMousePosition();
		}
		HODO_API HDMath::HDFLOAT2 GetMousePositionNormalized()
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
