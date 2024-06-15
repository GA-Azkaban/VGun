#pragma once
#include <windows.h>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "InputData.h"
#include "MathHeader.h"
#include "Singleton.h"
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
		void Initialize(HWND hWnd, HINSTANCE instance);
		void Update();
		void Finalize();

		bool GetKey(BYTE key);
		bool GetKeyDown(BYTE key);
		bool GetKeyUp(BYTE key);

		bool GetMouse(BYTE key);
		bool GetMouseDown(BYTE key);
		bool GetMouseUp(BYTE key);

		Vector2 GetMousePosition();
		Vector2 GetMouseDelta();

		char ConvertKeyToChar(BYTE key, bool isShiftPressed);
		char GetInputText(BYTE key);

		float GetMouseWheel();

		void Flush();
		void RecursiveMouse();

		void SetRecursiveMouseMode(bool isModeOn);

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

		bool					_isKeyPressed;
		bool					_isShiftPressed;

		POINT					_mousePos;
		POINT					_prevMousePos;
		POINT					_mouseDelta;
		POINT					_prevMouseDelta;
		int						_mouseWheel;
		int						_wheelMax;
		int						_wheelMin;

		// 무한마우스를 조건부로 하기 위해 추가. 24.2.1.AJY.
		bool					_isFirstPersonPerspective = true;
	};
}


