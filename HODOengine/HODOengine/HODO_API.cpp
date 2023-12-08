#include "HODO_API.h"

#include "SceneSystem.h"
#include "ObjectSystem.h"
#include "InputSystem.h"
#include "TimeSystem.h"
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
		HODO_API bool GetKeyDown(int keyCode)
		{
			return InputSystem::Instance().GetKeyDown(keyCode);
		}
		HODO_API bool GetKeyUp(int keyCode)
		{
			return InputSystem::Instance().GetKeyUp(keyCode);
		}
		HODO_API bool GetKeyPressing(int keyCode)
		{
			return InputSystem::Instance().GetKeyPressing(keyCode);
		}
		HODO_API HDMaths::HDFLOAT2 GetMousePosition()
		{
			return InputSystem::Instance().GetMousePosition();
		}
		HODO_API HDMaths::HDFLOAT2 GetMousePositionNormalized()
		{
			return InputSystem::Instance().GetMousePositionNormalized();
		}
		HODO_API float GetDeltaTime()
		{
			return TimeSystem::Instance().GetDeltaTime();
		}
	}
}
