#include "InputSystem.h"
#include <cassert>


namespace HDEngine
{
	void InputSystem::Initialize(HWND hWnd, HINSTANCE instance, int screenWidth, int screenHeight)
	{
		_hWnd = hWnd;
		_instance = instance;

		_screenWidth = screenWidth;
		_screenHeight = screenHeight;

		_widthOffset = (screenWidth - 1920) / 2;
		_heightOffset = 0;

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
		if (_mousePos.x < 0 || _mousePos.x > _screenWidth || _mousePos.y < 0 || _mousePos.y > _screenHeight)
		{
			_mousePos.x = 0;
			_mousePos.y = 0;
		}

		// 휠값 벗어나는 경우 보정
		if (_mouseWheel > _wheelMax)
		{
			_mouseWheel = _wheelMax;
		}
		if (_mouseWheel < _wheelMin)
		{
			_mouseWheel = _wheelMin;
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

	bool InputSystem::CheckMouseMove()
	{
		if (std::abs(_prevMousePos.x - _mousePos.x) > 2 ||
			std::abs(_prevMousePos.y != _mousePos.y) > 2)
		{
			return true;
		}
	}

	bool InputSystem::Check2DClicked(float x, float y, float width, float height)
	{
		if (_mousePos.x > x &&
			_mousePos.y > y &&
			_mousePos.x < x + width &&
			_mousePos.y < y + height)
		{
			return true;
		}
	}

	HDMath::HDFLOAT2 InputSystem::GetMousePosition()
	{
		float x = static_cast<float>(_mousePos.x);
		float y = static_cast<float>(_mousePos.y);

		return HDMath::HDFLOAT2{ x, y };
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

		RecursiveMouse();
	}

	void InputSystem::RecursiveMouse()
	{
		RECT windowRect;
		GetWindowRect(_hWnd, &windowRect);

		POINT mousePoint;

		
		LONG x = 0;
		LONG y = 0;

		/// 마우스 위치 이동 방식
		if (windowRect.right - 1 <= _mousePos.x)
		{
			x = windowRect.left + 2;
			y = _mousePos.y;
			mousePoint = { x, y };
			ScreenToClient(_hWnd, &mousePoint);
			_prevMousePos = { mousePoint.x - _widthOffset, mousePoint.y - _heightOffset };
			mouse_event(MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE, x * 65535 / GetSystemMetrics(SM_CXSCREEN), y * 65535 / GetSystemMetrics(SM_CYSCREEN), 0, 0);
		}
		else if (_mousePos.x <= windowRect.left + 1)
		{
			x = windowRect.right - 2;
			y = _mousePos.y;
			mousePoint = { x, y };
			ScreenToClient(_hWnd, &mousePoint);
			_prevMousePos = { mousePoint.x - _widthOffset, mousePoint.y - _heightOffset };
			mouse_event(MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE, x * 65535 / GetSystemMetrics(SM_CXSCREEN), y * 65535 / GetSystemMetrics(SM_CYSCREEN), 0, 0);
		}
		if (windowRect.bottom - 1 <= _mousePos.y)
		{
			x = _mousePos.x;
			y = windowRect.top + 2;
			mousePoint = { x, y };
			ScreenToClient(_hWnd, &mousePoint);
			_prevMousePos = { mousePoint.x - _widthOffset, mousePoint.y - _heightOffset };
			mouse_event(MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE, x * 65535 / GetSystemMetrics(SM_CXSCREEN), y * 65535 / GetSystemMetrics(SM_CYSCREEN), 0, 0);
		}
		else if (_mousePos.y <= windowRect.top + 1)
		{
			x = _mousePos.x;
			y = windowRect.bottom - 2;
			mousePoint = { x, y };
			ScreenToClient(_hWnd, &mousePoint);
			_prevMousePos = { mousePoint.x - _widthOffset, mousePoint.y - _heightOffset };
			mouse_event(MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE, x * 65535 / GetSystemMetrics(SM_CXSCREEN), y * 65535 / GetSystemMetrics(SM_CYSCREEN), 0, 0);
		}
	}
}

HDMath::HDFLOAT2 HDEngine::InputSystem::GetMouseDelta()
{
	_mouseDelta.x = _mousePos.x - _prevMousePos.x;
	_mouseDelta.y = _mousePos.y - _prevMousePos.y;

	HDMath::HDFLOAT2 result{};
	result.x = static_cast<float>((_mouseDelta.x + _prevMouseDelta.x) / 2.0f);
	result.y = static_cast<float>((_mouseDelta.y + _prevMouseDelta.y) / 2.0f);

	_prevMouseDelta = _mouseDelta;

	return result;
}
