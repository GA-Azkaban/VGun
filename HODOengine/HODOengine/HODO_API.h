﻿#pragma once
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING

/// <summary>
/// 오수안
/// 일단 임시로 하나의 API 클래스로 통합해둔다.
/// 추후에 기능별로 진입해서? 사용할 수 있도록 예쁘게 래핑할 예정
/// </summary>

#include "DLLExporter.h"

#include <windows.h>
#include <string>
#include "MathHeader.h"

#include "Scene.h"
#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "Camera.h"
#include "Script.h"
#include "Collider.h"
#include "StaticBoxCollider.h"
#include "DynamicBoxCollider.h"
#include "DynamicCapsuleCollider.h"
#include "DynamicSphereCollider.h"
#include "MeshRenderer.h"
#include "SkinnedMeshRenderer.h"
#include "InputData.h"
#include "TextUI.h"
#include "Button.h"
#include "ImageUI.h"
#include "SlideBoxUI.h"
#include "SliderUI.h"
#include "AudioSource.h"
#include "AudioClip.h"
#include "ToggleUI.h"
#include "TextInputBoxUI.h"

#include "ObjectSystem.h"

namespace API
{
	extern "C"
	{
		// 씬을 생성, 로드
		HODO_API HDData::Scene* CreateScene(std::string sceneName);
		HODO_API void LoadScene(HDData::Scene* scene);

		// 각종 오브젝트 생성
		HODO_API HDData::GameObject* CreateObject(HDData::Scene* scene, std::string objectName = "", HDData::GameObject* parentObject = nullptr);
		HODO_API HDData::GameObject* CreateImageBox(HDData::Scene* scene, std::string objectName = "", HDData::GameObject* parentObject = nullptr);
		HODO_API HDData::GameObject* CreateButton(HDData::Scene* scene, std::string objectName = "", HDData::GameObject* parentObject = nullptr);
		HODO_API HDData::GameObject* CreateTextbox(HDData::Scene* scene, std::string objectName = "", HDData::GameObject* parentObject = nullptr);
		HODO_API HDData::GameObject* CreateSlidebox(HDData::Scene* scene, std::string objectName = "", HDData::GameObject* parentObject = nullptr);
		HODO_API HDData::GameObject* CreateSlider(HDData::Scene* scene, int defaultValue, std::string objectName = "", HDData::GameObject* parentObject = nullptr);
		HODO_API HDData::GameObject* CreateToggle(HDData::Scene* scene, std::string objectName = "", HDData::GameObject* parentObject = nullptr);
		HODO_API HDData::GameObject* CreateTextInputBox(HDData::Scene* scene, std::string objectName = "", HDData::GameObject* parentObject = nullptr);


		// 키 입력을 위한 함수 (키보드, 마우스)
		HODO_API bool GetKeyDown(BYTE keyCode);
		HODO_API bool GetKeyUp(BYTE keyCode);
		HODO_API bool GetKeyPressing(BYTE keyCode);
		HODO_API bool GetMouseDown(int keyCode);
		HODO_API bool GetMouseUp(int keyCode);
		HODO_API bool GetMouseHold(int keyCode);
		HODO_API Vector2 GetMousePosition();
		HODO_API float GetMouseWheel();
		HODO_API Vector2 GetMouseDelta();

		// 디버그 시스템을 위한 함수
		HODO_API void DebugModeOn(int flag);
		HODO_API void DrawLine(Vector3 start, Vector3 end, Vector4 color);
		HODO_API void DrawLineDir(Vector3 start, Vector3 direction, float length, Vector4 color);

		// 델타 타임
		HODO_API float GetDeltaTime();

		/// physics stuff
		HODO_API HDData::Collider* ShootRay(Vector3 origin, Vector3 direction, float length = 100.0f, int* type = nullptr);
	}
}

