#include "InputSystem.h"
#include <cassert>
#include <ctype.h>


namespace HDEngine
{
	void InputSystem::Initialize(HWND hWnd, HINSTANCE instance, int screenWidth, int screenHeight)
	{
		_hWnd = hWnd;
		_instance = instance;

		_screenWidth = screenWidth;
		_screenHeight = screenHeight;

		_widthOffset = (screenWidth - 1920) / 2;
		_heightOffset = GetSystemMetrics(SM_CYCAPTION) + 8;	// window title bar height. 24.1.31.AJY

		_wheelMax = 500;
		_wheelMin = -500;

		StartDXInput();
	}

	void InputSystem::Update()
	{
		// 장치 제어권 체크
		if (_mouseDevice == NULL || _keyboardDevice == NULL)
		{
			assert(false, "no input device");
		}

		// 변수에 각 키값들의 input 상태 받아오기
		HRESULT hr;

		if (FAILED(hr = _keyboardDevice->GetDeviceState(256, _keyState)))
		{
			while (_keyboardDevice->Acquire() == DIERR_INPUTLOST);
		}
		if (FAILED(hr = _mouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &_DImouseState)))
		{
			while (_mouseDevice->Acquire() == DIERR_INPUTLOST);
		}

		// 마우스 상태 복사 (안전을 위해...)
		for (int i = 0; i < 3; i++)
		{
			_mouseState[i] = _DImouseState.rgbButtons[i];
		}

		// 스크린 좌표와 맞추기
		GetCursorPos(&_mousePos);
		ScreenToClient(_hWnd, &_mousePos);

		_mousePos.x += _DImouseState.lX;
		_mousePos.y += _DImouseState.lY;
		_mouseWheel += _DImouseState.lZ;

		// 윈도우 벗어나는 경우 좌표값 보정
		//if (_mousePos.x < 0 || _mousePos.x > _screenWidth || _mousePos.y < 0 || _mousePos.y > _screenHeight)
		//{
		//	_mousePos.x = 0;
		//	_mousePos.y = 0;
		//}

		if (GetKeyDown(DIK_O))
		{
			_isFirstPersonPerspective = !_isFirstPersonPerspective;
		}
	}

	void InputSystem::Finalize()
	{
		FinishDXInput();
	}

	bool InputSystem::StartDXInput()
	{
		HRESULT hr = S_OK;

		// 장치 초기화
		if (FAILED(hr = DirectInput8Create(_instance,
			DIRECTINPUT_VERSION,
			IID_IDirectInput8,
			(void**)&_DI, NULL)))
		{
			return false;
		}

		// 키보드 연결
		if (FAILED(hr = _DI->CreateDevice(GUID_SysKeyboard,
			&_keyboardDevice, NULL)))
		{
			return false;
		}

		// 마우스 연결
		if (FAILED(hr = _DI->CreateDevice(GUID_SysMouse,
			&_mouseDevice, NULL)))
		{
			return false;
		}

		// 장치별 반환 데이터 설정
		_keyboardDevice->SetDataFormat(&c_dfDIKeyboard);
		_mouseDevice->SetDataFormat(&c_dfDIMouse);


		// 장치별 독점 및 비독점 설정(협조레벨)
		if (FAILED(hr = _keyboardDevice->SetCooperativeLevel(
			_hWnd,
			DISCL_NONEXCLUSIVE |
			DISCL_FOREGROUND |
			DISCL_NOWINKEY)))
		{
			return false;
		}
		while (_keyboardDevice->Acquire() == DIERR_INPUTLOST);


		if (FAILED(hr = _mouseDevice->SetCooperativeLevel(
			_hWnd,
			DISCL_NONEXCLUSIVE |
			DISCL_FOREGROUND)))
		{
			return false;
		}
		while (_mouseDevice->Acquire() == DIERR_INPUTLOST);

		return true;
	}

	bool InputSystem::FinishDXInput()
	{
		if (_mouseDevice)
		{
			_mouseDevice->Unacquire();
			_mouseDevice->Release();
			_mouseDevice = NULL;
		}
		if (_keyboardDevice)
		{
			_keyboardDevice->Unacquire();
			_keyboardDevice->Release();
			_keyboardDevice = NULL;
		}
		if (_DI)
		{
			_DI->Release();
			_DI = NULL;
		}

		return true;
	}

	bool InputSystem::GetKey(BYTE key)
	{
		return (_keyState[key] & 0x80) && (_prevKeyState[key] & 0x80);
	}

	bool InputSystem::GetKeyDown(BYTE key)
	{
		return _keyState[key] && _prevKeyState[key] == false;
	}

	bool InputSystem::GetKeyUp(BYTE key)
	{
		return _keyState[key] == false && _prevKeyState[key];
	}

	bool InputSystem::GetMouse(BYTE key)
	{
		return _mouseState[key] && _prevMouseState[key];
	}

	bool InputSystem::GetMouseDown(BYTE key)
	{
		return _mouseState[key] && _prevMouseState[key] == false;
	}

	bool InputSystem::GetMouseUp(BYTE key)
	{
		return _mouseState[key] == false && _prevMouseState[key];
	}

	Vector2 InputSystem::GetMousePosition()
	{
		float x = static_cast<float>(_mousePos.x);
		float y = static_cast<float>(_mousePos.y);

		return Vector2{ x, y };
	}

	float InputSystem::GetMouseWheel()
	{
		return _mouseWheel;
	}

	void InputSystem::Flush()
	{
		for (int i = 0; i < 256; ++i)
		{
			_prevKeyState[i] = _keyState[i];
			_keyState[i] = false;
		}

		for (int i = 0; i < 3; ++i)
		{
			_prevMouseState[i] = _mouseState[i];
			_mouseState[i] = false;
		}

		_prevMousePos = _mousePos;
		_isKeyPressed = false;

		if (_isFirstPersonPerspective)
		{
			RecursiveMouse();
		}
	}

	void InputSystem::RecursiveMouse()
	{
		RECT windowRect;
		GetWindowRect(_hWnd, &windowRect);

		/// when cursor get out of the window
		if (_mousePos.x >= _screenWidth)
		{
			_prevMousePos = { 0, _mousePos.y };
			SetCursorPos(windowRect.left + 7, _mousePos.y);
		}
		else if (_mousePos.x <= 0)
		{
			_prevMousePos = { _screenWidth, _mousePos.y };
			SetCursorPos(windowRect.right - 7, _mousePos.y);
		}
		if (_mousePos.y >= _screenHeight)
		{
			_prevMousePos = { _mousePos.x, 10};
			SetCursorPos(_mousePos.x, windowRect.top + 10);
		}
		else if (_mousePos.y <= 0)
		{
			_prevMousePos = { _mousePos.x, _screenHeight };
			SetCursorPos(_mousePos.x, windowRect.bottom - 10);
		}
	}
}

Vector2 HDEngine::InputSystem::GetMouseDelta()
{
	_mouseDelta.x = _mousePos.x - _prevMousePos.x;
	_mouseDelta.y = _mousePos.y - _prevMousePos.y;

	Vector2 result{};
	result.x = static_cast<float>((_mouseDelta.x + _prevMouseDelta.x) / 2.0f);
	result.y = static_cast<float>((_mouseDelta.y + _prevMouseDelta.y) / 2.0f);

	_prevMouseDelta = _mouseDelta;

	return result;
}

char HDEngine::InputSystem::ConvertKeyToChar(BYTE key, bool isShiftPressed)
{
		switch (key)
		{
			case DIK_1: {return _isShiftPressed ? '!' : '1'; } break;
			case DIK_2: {return _isShiftPressed ? '@' : '2'; } break;
			case DIK_3: {return _isShiftPressed ? '#' : '3'; } break;
			case DIK_4: {return _isShiftPressed ? '$' : '4'; } break;
			case DIK_5: {return _isShiftPressed ? '%' : '5'; } break;
			case DIK_6: {return _isShiftPressed ? '^' : '6'; } break;
			case DIK_7: {return _isShiftPressed ? '&' : '7'; } break;
			case DIK_8: {return _isShiftPressed ? '*' : '8'; } break;
			case DIK_9: {return _isShiftPressed ? '(' : '9'; } break;
			case DIK_0: {return _isShiftPressed ? ')' : '0'; } break;
			case DIK_MINUS: {return _isShiftPressed ? '_' : '-'; } break;
			case DIK_EQUALS: {return _isShiftPressed ? '+' : '='; } break;
			case DIK_SEMICOLON: {return _isShiftPressed ? ':' : ';'; } break;
			case DIK_APOSTROPHE: {return _isShiftPressed ? '"' : '\''; } break;
			case DIK_GRAVE: {return _isShiftPressed ? '~' : '`'; } break;
			case DIK_BACKSLASH: {return _isShiftPressed ? '|' : '\\'; } break;
			case DIK_COMMA: {return _isShiftPressed ? '<' : ','; } break;
			case DIK_PERIOD: {return _isShiftPressed ? '>' : '.'; } break;
			case DIK_SLASH: {return _isShiftPressed ? '?' : '/'; } break;
			case DIK_LBRACKET: {return _isShiftPressed ? '{' : '['; } break;
			case DIK_RBRACKET: {return _isShiftPressed ? '}' : ']'; } break;
			case DIK_Q : {return _isShiftPressed ? 'Q' : 'q'; } break;
			case DIK_W: {return _isShiftPressed ? 'W' : 'w'; } break;
			case DIK_E: {return _isShiftPressed ? 'E' : 'e'; } break;
			case DIK_R: {return _isShiftPressed ? 'R' : 'r'; } break;
			case DIK_T: {return _isShiftPressed ? 'T' : 't'; } break;
			case DIK_Y: {return _isShiftPressed ? 'Y' : 'y'; } break;
			case DIK_U: {return _isShiftPressed ? 'U' : 'u'; } break;
			case DIK_I: {return _isShiftPressed ? 'I' : 'i'; } break;
			case DIK_O: {return _isShiftPressed ? 'O' : 'o'; } break;
			case DIK_P: {return _isShiftPressed ? 'P' : 'p'; } break;
			case DIK_A: {return _isShiftPressed ? 'A' : 'a'; } break;
			case DIK_S: {return _isShiftPressed ? 'S' : 's'; } break;
			case DIK_D: {return _isShiftPressed ? 'D' : 'd'; } break;
			case DIK_F: {return _isShiftPressed ? 'F' : 'f'; } break;
			case DIK_G: {return _isShiftPressed ? 'G' : 'g'; } break;
			case DIK_H: {return _isShiftPressed ? 'H' : 'h'; } break;
			case DIK_J: {return _isShiftPressed ? 'J' : 'j'; } break;
			case DIK_K: {return _isShiftPressed ? 'K' : 'k'; } break;
			case DIK_L: {return _isShiftPressed ? 'L' : 'l'; } break;
			case DIK_Z: {return _isShiftPressed ? 'Z' : 'z'; } break;
			case DIK_X: {return _isShiftPressed ? 'X' : 'x'; } break;
			case DIK_C: {return _isShiftPressed ? 'C' : 'c'; } break;
			case DIK_V: {return _isShiftPressed ? 'V' : 'v'; } break;
			case DIK_B: {return _isShiftPressed ? 'B' : 'b'; } break;
			case DIK_N: {return _isShiftPressed ? 'N' : 'n'; } break;
			case DIK_M: {return _isShiftPressed ? 'M' : 'm'; } break;
			default:
			{
				return '?';
			}
			break;
		}
}

char HDEngine::InputSystem::GetInputText(BYTE key)
{
	// shift 키 여부
	_isShiftPressed = (_keyState[DIK_LSHIFT] & 0x80) || (_keyState[DIK_RSHIFT] & 0x80);

	return ConvertKeyToChar(key, _isShiftPressed);

}
