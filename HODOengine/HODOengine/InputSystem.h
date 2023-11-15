#pragma once
#include <windows.h>

#include "../HODOmath/HODOmath.h"
#include "Singleton.h"
/// <summary>
/// 오수안
/// 키 인풋을 위한 기초 시스템
/// </summary>

namespace hodoEngine
{

	class InputSystem : public Singleton<InputSystem>
	{
		friend Singleton;
	private:
		InputSystem();

	public:
		void Initialize(int hWnd, int screenWidth, int screenHeight);
		void Update();

	private:
		HWND _hWnd;

	public:
		bool GetKeyDown(int keyCode);
		bool GetKeyUp(int keyCode);
		bool GetKeyPressing(int keyCode);
		
	private:
		bool _currentKeyState[256];
		bool _previousKeyState[256];

	public:
		POINT GetMousePosition();

	private:
		POINT _mousePosition;
		HDMaths::HDFLOAT2 _clientMousePos;
		int _screenWidth;
		int _screenHeight;

	};
}


