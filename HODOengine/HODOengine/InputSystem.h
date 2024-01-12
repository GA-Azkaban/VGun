#pragma once
#include <windows.h>
#include "InputData.h"
#include "../HODOmath/HODOmath.h"
#include "Singleton.h"
#include <unordered_map>
#include <unordered_set>
/// <summary>
/// 오수안
/// DirectInput을 이용한 개선된 인풋 시스템
/// </summary>



namespace HDEngine
{
	class InputSystem : public Singleton<InputSystem>
	{
		friend Singleton;

	private:
		InputSystem() = default;

	public:
		void Initialize(HWND hWnd, HINSTANCE instance, int screenWidth, int screenHeight);
		void Update();
		void Finalize();

		bool GetKey(BYTE key);
		bool GetKeyDown(BYTE key);
		bool GetKeyUp(BYTE key);

		bool GetMouse(BYTE key);
		bool GetMouseDown(BYTE key);
		bool GetMouseUp(BYTE key);

		HDMath::HDFLOAT2 GetMousePosition();
		HDMath::HDFLOAT2 GetMousePositionNormalized();
		HDMath::HDFLOAT2 GetMouseDelta();
	
		float GetMouseWheel();

		void Flush();

	private:
		bool StartDXInput();
		bool FinishDXInput();

	private:
		HWND					_hWnd;
		HINSTANCE				_instance;

		int						_screenWidth;
		int						_screenHeight;

		LPDIRECTINPUT8			_DI;
		LPDIRECTINPUTDEVICE8	_keyboardDevice;
		LPDIRECTINPUTDEVICE8	_mouseDevice;

		DIMOUSESTATE			_DImouseState;
		bool					_mouseState[3];
		bool					_prevMouseState[3];
		bool					_keyState[256];
		bool					_prevKeyState[256];

		POINT					_mousePos;
		int						_mouseWheel;
		int						_wheelMax;
		int						_wheelMin;
		HDMath::HDFLOAT2 _currentMousePosition;
		HDMath::HDFLOAT2 _previousMousePosition;
		HDMath::HDFLOAT2 _currentMouseDelta;
		HDMath::HDFLOAT2 _previousMouseDelta;
	};
}


