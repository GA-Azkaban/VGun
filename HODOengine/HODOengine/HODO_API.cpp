#include "HODO_API.h"

#include "SceneSystem.h"
#include "ObjectSystem.h"
#include "InputSystem.h"
#include "TimeSystem.h"
#include "GameObject.h"
#include "DebugSystem.h"
#include "PhysicsSystem.h"
#include "RenderSystem.h"
#include "SceneLoader.h"

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

		HODO_API HDData::GameObject* CreateStaticObject(std::string objectName /*= ""*/, HDData::GameObject* parentObject /*= nullptr*/)
		{
			auto obj = HDEngine::ObjectSystem::Instance().CreateStaticObject(objectName, parentObject);
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

		HODO_API HDData::GameObject* CreateSlidebox(HDData::Scene* scene, std::string objectName /*= ""*/, HDData::GameObject* parentObject /*= nullptr*/)
		{
			auto obj = HDEngine::ObjectSystem::Instance().CreateObject(scene, objectName, parentObject);
			obj->AddComponent<HDData::SlideBoxUI>();

			auto leftButton = HDEngine::ObjectSystem::Instance().CreateObject(scene, "arrowLeft", obj);
			leftButton->AddComponent<HDData::ImageUI>();
			leftButton->GetComponent<HDData::ImageUI>()->SetImage("arrowLeft.png");

			auto rightButton = HDEngine::ObjectSystem::Instance().CreateObject(scene, "arrowRight", obj);
			rightButton->AddComponent<HDData::ImageUI>();
			rightButton->GetComponent<HDData::ImageUI>()->SetImage("arrowRight.png");

			auto valueText = HDEngine::ObjectSystem::Instance().CreateObject(scene, "Text", obj);
			valueText->AddComponent<HDData::TextUI>();
			valueText->GetComponent<HDData::TextUI>()->SetIsIgnoreFocus(true);
			valueText->GetComponent<HDData::TextUI>()->SetText("default");
			
			return obj;
		}

		HODO_API HDData::GameObject* CreateToggle(HDData::Scene* scene, std::string objectName /*= ""*/, HDData::GameObject* parentObject /*= nullptr*/)
		{
			auto obj = HDEngine::ObjectSystem::Instance().CreateObject(scene, objectName, parentObject);
			obj->AddComponent<HDData::ToggleUI>();

			auto toggleOn = HDEngine::ObjectSystem::Instance().CreateObject(scene, "toggleOn" , obj);
			toggleOn->AddComponent<HDData::ImageUI>();
			toggleOn->GetComponent<HDData::ImageUI>()->SetImage("Sound.png");

			auto toggleOff = HDEngine::ObjectSystem::Instance().CreateObject(scene, "toggleOff" , obj);
			toggleOff->AddComponent<HDData::ImageUI>();
			toggleOff->GetComponent<HDData::ImageUI>()->SetImage("Mute.png");

			return obj;
		}

		HODO_API HDData::GameObject* CreateSlider(HDData::Scene* scene, int defaultValue, std::string objectName /*= ""*/, HDData::GameObject* parentObject /*= nullptr*/)
		{
			auto obj = HDEngine::ObjectSystem::Instance().CreateObject(scene, objectName, parentObject);
			obj->AddComponent<HDData::SliderUI>();

			auto background = HDEngine::ObjectSystem::Instance().CreateObject(scene, "bar", obj);
			background->AddComponent<HDData::ImageUI>();
			background->GetComponent<HDData::ImageUI>()->SetImage("bar.png");

			auto fill = HDEngine::ObjectSystem::Instance().CreateObject(scene, "fill", obj);
			fill->AddComponent<HDData::ImageUI>();
			fill->GetComponent<HDData::ImageUI>()->SetImage("fill.png");
			fill->GetComponent<HDData::ImageUI>()->SetIsIgnoreFocus(true);

			auto handle = HDEngine::ObjectSystem::Instance().CreateObject(scene, "handle", obj);
			handle->AddComponent<HDData::ImageUI>();
			handle->GetComponent<HDData::ImageUI>()->SetImage("point.png");
			handle->GetComponent<HDData::ImageUI>()->SetIsIgnoreFocus(true);

			auto valueText = HDEngine::ObjectSystem::Instance().CreateObject(scene, "value", obj);
			valueText->AddComponent<HDData::TextUI>();
			valueText->GetComponent<HDData::TextUI>()->SetText(std::to_string(defaultValue));
			valueText->GetComponent<HDData::TextUI>()->SetIsIgnoreFocus(true);

			return obj;
		}

		HODO_API HDData::GameObject* CreateTextInputBox(HDData::Scene* scene, std::string objectName /*= ""*/, HDData::GameObject* parentObject /*= nullptr*/)
		{
			auto obj = HDEngine::ObjectSystem::Instance().CreateObject(scene, objectName, parentObject);
			obj->AddComponent<HDData::TextInputBoxUI>();

			auto background = HDEngine::ObjectSystem::Instance().CreateObject(scene, "back", obj);
			background->AddComponent<HDData::ImageUI>();
			background->GetComponent<HDData::ImageUI>()->SetImage("back.png");

			auto cursor = HDEngine::ObjectSystem::Instance().CreateObject(scene, "cursor", obj);
			cursor->AddComponent<HDData::ImageUI>();
			cursor->GetComponent<HDData::ImageUI>()->SetImage("cursor.png");
			cursor->GetComponent<HDData::ImageUI>()->SetIsIgnoreFocus(true);

			auto valueText = HDEngine::ObjectSystem::Instance().CreateObject(scene, "text", obj);
			valueText->AddComponent<HDData::TextUI>();
			valueText->GetComponent<HDData::TextUI>()->SetText("");
			valueText->GetComponent<HDData::TextUI>()->SetColor(DirectX::Colors::Black);

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

		HODO_API Vector2 GetMousePosition()
		{
			return HDEngine::InputSystem::Instance().GetMousePosition();
		}

		HODO_API float GetMouseWheel()
		{
			return HDEngine::InputSystem::Instance().GetMouseWheel();
		}

		HODO_API Vector2 GetMouseDelta()
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

		HODO_API HDData::Collider* ShootRay(Vector3 origin, Vector3 direction, float length /*= 100.0f*/, int* type /*= nullptr*/)
		{
			return HDEngine::PhysicsSystem::Instance().RayCast(origin.x, origin.y, origin.z, direction.x, direction.y, direction.z, length, type);
		}

		HODO_API HDData::Collider* ShootRayHitPoint(Vector3 origin, Vector3 direction, Vector3& hitPoint, float length /*= 100.0f*/, int* type /*= nullptr*/)
		{
			return HDEngine::PhysicsSystem::Instance().RayCastHitPoint(origin.x, origin.y, origin.z, direction.x, direction.y, direction.z, hitPoint, length, type);
		}

		HODO_API void DrawLine(Vector3 start, Vector3 end, Vector4 color)
		{
			HDEngine::RenderSystem::Instance().DrawLine(start, end, color);
		}

		HODO_API void DrawLineDir(Vector3 start, Vector3 direction, float length, Vector4 color)
		{
			HDEngine::RenderSystem::Instance().DrawLine(start, direction, length, color);
		}

		HODO_API void LoadSceneFromData(std::string fileName)
		{
			HDEngine::SceneLoader::Instance().LoadUnityScene(fileName);
		}

		HODO_API HDData::Camera* GetMainCamera()
		{
			return HDEngine::SceneSystem::Instance().GetCurrentScene()->GetMainCamera();
		}

		HODO_API HDData::Camera* SetMainCamera(HDData::Camera* camera)
		{
			HDData::Camera* prevCam = GetMainCamera();
			HDEngine::SceneSystem::Instance().GetCurrentScene()->SetMainCamera(camera);
			return prevCam;
		}
	}



}
