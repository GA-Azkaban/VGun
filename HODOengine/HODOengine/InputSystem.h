#pragma once
#include <windows.h>

#include "../HODOmath/HODOmath.h"
#include "Singleton.h"
#include <unordered_map>
#include <unordered_set>
/// <summary>
/// 오수안
/// 키 인풋을 위한 기초 시스템
/// </summary>

namespace HDEngine
{

	class InputSystem : public Singleton<InputSystem>
	{
		friend Singleton;
	private:
		InputSystem() = default;

	public:
		void Initialize();
		void Update();

	public:
		bool GetKeyDown(int keyCode);
		bool GetKeyUp(int keyCode);
		bool GetKeyPressing(int keyCode);

	public:
		HDMath::HDFLOAT2 GetMousePosition();
		HDMath::HDFLOAT2 GetMousePositionNormalized();
		
	private:
		bool _currentKeyState[256];
		bool _previousKeyState[256];
	};
}


