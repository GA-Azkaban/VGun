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

		HODO_API void CreateStaticComponent(HDData::Component* component)
		{
			HDEngine::ObjectSystem::Instance().CreateStaticComponent(component);
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
			auto slide = obj->AddComponent<HDData::SlideBoxUI>();

			auto leftButton = HDEngine::ObjectSystem::Instance().CreateObject(scene, "arrowLeft", obj);
			auto l = leftButton->AddComponent<HDData::ImageUI>();
			l->SetImage("arrowLeft.png");
			slide->SetLeftComp(l);

			auto rightButton = HDEngine::ObjectSystem::Instance().CreateObject(scene, "arrowRight", obj);
			auto r = rightButton->AddComponent<HDData::ImageUI>();
			r->SetImage("arrowRight.png");
			slide->SetRightComp(r);

			auto valueText = HDEngine::ObjectSystem::Instance().CreateObject(scene, "Text", obj);
			auto val = valueText->AddComponent<HDData::TextUI>();
			val->SetIsIgnoreFocus(true);
			val->SetText("default");
			slide->SetValueComp(val);
			
			return obj;
		}

		HODO_API HDData::GameObject* CreateToggle(HDData::Scene* scene, std::string objectName /*= ""*/, HDData::GameObject* parentObject /*= nullptr*/)
		{
			auto obj = HDEngine::ObjectSystem::Instance().CreateObject(scene, objectName, parentObject);
			auto toggle = obj->AddComponent<HDData::ToggleUI>();

			auto toggleOn = HDEngine::ObjectSystem::Instance().CreateObject(scene, "toggleOn" , obj);
			auto on = toggleOn->AddComponent<HDData::ImageUI>();
			on->SetImage("Sound.png");
			toggle->SetOnComp(on);

			auto toggleOff = HDEngine::ObjectSystem::Instance().CreateObject(scene, "toggleOff" , obj);
			auto off = toggleOff->AddComponent<HDData::ImageUI>();
			off->SetImage("Mute.png");
			toggle->SetOffComp(off);

			return obj;
		}

		HODO_API HDData::GameObject* CreateSlider(HDData::Scene* scene, int defaultValue, std::string objectName /*= ""*/, HDData::GameObject* parentObject /*= nullptr*/)
		{
			auto obj = HDEngine::ObjectSystem::Instance().CreateObject(scene, objectName, parentObject);
			auto p = obj->AddComponent<HDData::SliderUI>();

			auto background = HDEngine::ObjectSystem::Instance().CreateObject(scene, "bar", obj);
			auto back = background->AddComponent<HDData::ImageUI>();
			back->SetImage("bar.png");
			p->SetBackBarComp(back);

			auto fill = HDEngine::ObjectSystem::Instance().CreateObject(scene, "fill", obj);
			auto f = fill->AddComponent<HDData::ImageUI>();
			f->SetImage("fill.png");
			f->SetIsIgnoreFocus(true);
			p->SetFillBarComp(f);

			auto handle = HDEngine::ObjectSystem::Instance().CreateObject(scene, "handle", obj);
			auto h = handle->AddComponent<HDData::ImageUI>();
			h->SetImage("point.png");
			h->SetIsIgnoreFocus(true);
			p->SetHandleComp(h);

			auto valueText = HDEngine::ObjectSystem::Instance().CreateObject(scene, "value", obj);
			auto text = valueText->AddComponent<HDData::TextUI>();
			text->SetText(std::to_string(defaultValue));
			text->SetIsIgnoreFocus(true);
			p->SetValueComp(text);

			return obj;
		}

		HODO_API HDData::GameObject* CreateTextInputBox(HDData::Scene* scene, std::string objectName /*= ""*/, HDData::GameObject* parentObject /*= nullptr*/)
		{
			auto obj = HDEngine::ObjectSystem::Instance().CreateObject(scene, objectName, parentObject);
			auto p = obj->AddComponent<HDData::TextInputBoxUI>();

			auto background = HDEngine::ObjectSystem::Instance().CreateObject(scene, "back", obj);
			auto back = background->AddComponent<HDData::ImageUI>();
			back->SetImage("back.png");
			p->SetBoxComp(back);

			auto cursor = HDEngine::ObjectSystem::Instance().CreateObject(scene, "cursor", obj);
			auto cur = cursor->AddComponent<HDData::ImageUI>();
			cur->SetImage("cursor.png");
			cur->SetIsIgnoreFocus(true);
			p->SetCursorComp(cur);

			auto valueText = HDEngine::ObjectSystem::Instance().CreateObject(scene, "text", obj);
			auto val = valueText->AddComponent<HDData::TextUI>();
			val->SetText("");
			val->SetColor(DirectX::Colors::Black);
			p->SetTextComp(val);

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
