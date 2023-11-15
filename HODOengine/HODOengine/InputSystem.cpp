#include "InputSystem.h"

#include <windows.h>


void hodoEngine::InputSystem::Initialize(int hWnd, int screenWidth, int screenHeight)
{
	_hWnd = (HWND)hWnd;
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;

	for (int i = 0; i < 256; ++i)
	{
		_currentKeyState[i] = false;
	}
}

void hodoEngine::InputSystem::Update()
{
	// 현재 키 상태를 계속 체크한다
	for (int keyCode = 0; keyCode < 256; ++keyCode)
	{
		_currentKeyState[keyCode] = GetAsyncKeyState(keyCode);
	}

	// 현재 마우스 좌표를 계속 받아온다
	GetCursorPos(&_mousePosition);
	// TODO) 클라이언트 크기에 맞춰야 함
}

bool hodoEngine::InputSystem::GetKeyDown(int keyCode)
{
	return !_previousKeyState[keyCode] && _currentKeyState[keyCode];
}

bool hodoEngine::InputSystem::GetKeyUp(int keyCode)
{
	return _previousKeyState[keyCode] && !_currentKeyState[keyCode];
}

bool hodoEngine::InputSystem::GetKeyPressing(int keyCode)
{
	return _previousKeyState[keyCode] && _currentKeyState[keyCode];
}

POINT hodoEngine::InputSystem::GetMousePosition()
{
	return _mousePosition;
}
