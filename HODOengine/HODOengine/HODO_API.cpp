#include "HODO_API.h"

#include "SceneSystem.h"
#include "ObjectSystem.h"
#include "InputSystem.h"
#include "TimeSystem.h"
#include "GameObject.h"
#include "DebugSystem.h"
#include "PhysicsSystem.h"

namespace API
{
	extern "C"
	{
		HODO_API HDData::Scene* CreateScene(std::string sceneName)
		{
			return HDEngine::SceneSystem::Instance().CreateScene(sceneName);
		}
		HODO_API void LoadScene(HDData::Scene* scene)
		{
			HDEngine::SceneSystem::Instance().LoadScene(scene);
		}
		HODO_API HDData::GameObject* CreateObject(HDData::Scene* scene, std::string objectName, HDData::GameObject* parentObject)
		{
			auto obj = HDEngine::ObjectSystem::Instance().CreateObject(scene, objectName, parentObject);

			return obj;
		}
		HODO_API bool GetKeyDown(int keyCode)
		{
			return HDEngine::InputSystem::Instance().GetKeyDown(keyCode);
		}
		HODO_API bool GetKeyUp(int keyCode)
		{
			return HDEngine::InputSystem::Instance().GetKeyUp(keyCode);
		}
		HODO_API bool GetKeyPressing(int keyCode)
		{
			return HDEngine::InputSystem::Instance().GetKeyPressing(keyCode);
		}
		HODO_API HDMath::HDFLOAT2 GetMousePosition()
		{
			return HDEngine::InputSystem::Instance().GetMousePosition();
		}
		HODO_API HDMath::HDFLOAT2 GetMousePositionNormalized()
		{
			return HDEngine::InputSystem::Instance().GetMousePositionNormalized();
		}
		HODO_API float GetDeltaTime()
		{
			return HDEngine::TimeSystem::Instance().GetDeltaTime();
		}
		HODO_API void DebugModeOn(int flag)
		{
			HDEngine::DebugSystem::Instance().SetDebugOn(flag);
		}
		HODO_API HDData::Collider* ShootRay(HDMath::HDFLOAT3 origin, HDMath::HDFLOAT3 direction, float length /*= 100.0f*/, int* type /*= nullptr*/)
		{
			return HDEngine::PhysicsSystem::Instance().RayCast(origin.x, origin.y, origin.z, direction.x, direction.y, direction.z, length, type);
		}
	}
}
