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
	// ���� Ű ���¸� ��� üũ�Ѵ�
	for (int keyCode = 0; keyCode < 256; ++keyCode)
	{
		_currentKeyState[keyCode] = GetAsyncKeyState(keyCode);
	}

	// ���� ���콺 ��ǥ�� ��� �޾ƿ´�
	GetCursorPos(&_mousePosition);
	// TODO) Ŭ���̾�Ʈ ũ�⿡ ����� ��
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
