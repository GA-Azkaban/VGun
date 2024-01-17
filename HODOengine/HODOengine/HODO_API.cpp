#include "HODO_API.h"

#include "SceneSystem.h"
#include "ObjectSystem.h"
#include "InputSystem.h"
#include "TimeSystem.h"
#include "GameObject.h"
#include "DebugSystem.h"
#include "PhysicsSystem.h"
#include "RenderSystem.h"

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

		HODO_API HDData::GameObject* CreateImageBox(HDData::Scene* scene, std::string objectName /*= ""*/, HDData::GameObject* parentObject /*= nullptr*/)
		{
			auto obj = HDEngine::ObjectSystem::Instance().CreateObject(scene, objectName, parentObject);
			obj->AddComponent<HDData::ImageUI>();
			return obj;
		}

		HODO_API HDData::GameObject* CreateButton(HDData::Scene* scene, std::string objectName /*= ""*/, HDData::GameObject* parentObject /*= nullptr*/)
		{
			auto obj = HDEngine::ObjectSystem::Instance().CreateObject(scene, objectName, parentObject);
			obj->AddComponent<HDData::Button>();
			return obj;
		}

		HODO_API HDData::GameObject* CreateTextbox(HDData::Scene* scene, std::string objectName /*= ""*/, HDData::GameObject* parentObject /*= nullptr*/)
		{
			auto obj = HDEngine::ObjectSystem::Instance().CreateObject(scene, objectName, parentObject);
			obj->AddComponent<HDData::TextUI>();
			return obj;
		}

		//HODO_API HDData::GameObject* CreateSlidebox(HDData::Scene* scene, std::string objectName /*= ""*/, HDData::GameObject* parentObject /*= nullptr*/)
		//{
		//	auto obj = HDEngine::ObjectSystem::Instance().CreateObject(scene, objectName, parentObject);
		//	obj->AddComponent<HDData::SlideBoxUI>();
		//	return obj;
		//}

		HODO_API HDData::GameObject* CreateToggle(HDData::Scene* scene, std::string objectName /*= ""*/, HDData::GameObject* parentObject /*= nullptr*/)
		{
			auto obj = HDEngine::ObjectSystem::Instance().CreateObject(scene, objectName, parentObject);
			obj->AddComponent<HDData::ToggleUI>();
			return obj;
		}

		HODO_API HDData::GameObject* CreateSlider(HDData::Scene* scene, std::string objectName /*= ""*/, HDData::GameObject* parentObject /*= nullptr*/)
		{
			auto obj = HDEngine::ObjectSystem::Instance().CreateObject(scene, objectName, parentObject);
			obj->AddComponent<HDData::SliderUI>();

			auto background = HDEngine::ObjectSystem::Instance().CreateObject(scene, "bar", obj);
			background->AddComponent<HDData::ImageUI>();
			background->GetComponent<HDData::ImageUI>()->SetImage("bar.png");

			auto fill = HDEngine::ObjectSystem::Instance().CreateObject(scene, "fill", obj);
			fill->AddComponent<HDData::ImageUI>();
			fill->GetComponent<HDData::ImageUI>()->SetImage("fill.png");

			auto handle = HDEngine::ObjectSystem::Instance().CreateObject(scene, "handle", obj);
			handle->AddComponent<HDData::ImageUI>();
			handle->GetComponent<HDData::ImageUI>()->SetImage("point.png");

			auto valueText = HDEngine::ObjectSystem::Instance().CreateObject(scene, "value", obj);
			valueText->AddComponent<HDData::TextUI>();
			valueText->GetComponent<HDData::TextUI>()->SetText("50");

			return obj;
		}

		HODO_API bool GetKeyDown(BYTE keyCode)
		{
			return HDEngine::InputSystem::Instance().GetKeyDown(keyCode);
		}

		HODO_API bool GetKeyUp(BYTE keyCode)
		{
			return HDEngine::InputSystem::Instance().GetKeyUp(keyCode);
		}

		HODO_API bool GetKeyPressing(BYTE keyCode)
		{
			return HDEngine::InputSystem::Instance().GetKey(keyCode);
		}

		HODO_API bool GetMouseDown(int keyCode)
		{
			return HDEngine::InputSystem::Instance().GetMouseDown(keyCode);
		}

		HODO_API bool GetMouseUp(int keyCode)
		{
			return HDEngine::InputSystem::Instance().GetMouseUp(keyCode);
		}

		HODO_API bool GetMouseHold(int keyCode)
		{
			return HDEngine::InputSystem::Instance().GetMouse(keyCode);
		}

		HODO_API HDMath::HDFLOAT2 GetMousePosition()
		{
			return HDEngine::InputSystem::Instance().GetMousePosition();
		}

		HODO_API float GetMouseWheel()
		{
			return HDEngine::InputSystem::Instance().GetMouseWheel();
		}

		HODO_API HDMath::HDFLOAT2 GetMouseDelta()
		{
			return HDEngine::InputSystem::Instance().GetMouseDelta();
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

		HODO_API void DrawLine(HDMath::HDFLOAT3 start, HDMath::HDFLOAT3 end, HDMath::HDFLOAT4 color)
		{
			HDEngine::RenderSystem::Instance().DrawLine(start, end, color);
		}

		HODO_API void DrawLineDir(HDMath::HDFLOAT3 start, HDMath::HDFLOAT3 direction, float length, HDMath::HDFLOAT4 color)
		{
			HDEngine::RenderSystem::Instance().DrawLine(start, direction, length, color);
		}
	}



}
