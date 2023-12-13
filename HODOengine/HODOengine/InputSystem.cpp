#include "InputSystem.h"
#include "HODOengine.h"
#include <windows.h>


void HDEngine::InputSystem::Initialize()
{
	for (int keyCode = 0; keyCode < 256; ++keyCode)
	{
		_previousKeyState[keyCode] = false;
		_currentKeyState[keyCode] = false;
	}
}

void HDEngine::InputSystem::Update()
{
	for (int keyCode = 0; keyCode < 256; ++keyCode)
	{
		_previousKeyState[keyCode] = _currentKeyState[keyCode];
	}
	// 현재 키 상태를 계속 체크한다
	for (int keyCode = 0; keyCode < 256; ++keyCode)
	{
		_currentKeyState[keyCode] = GetAsyncKeyState(keyCode);
	}
}

bool HDEngine::InputSystem::GetKeyDown(int keyCode)
{
	return !_previousKeyState[keyCode] && _currentKeyState[keyCode];
}

bool HDEngine::InputSystem::GetKeyUp(int keyCode)
{
	return _previousKeyState[keyCode] && !_currentKeyState[keyCode];
}

bool HDEngine::InputSystem::GetKeyPressing(int keyCode)
{
	return _previousKeyState[keyCode] && _currentKeyState[keyCode];
}

HDMath::HDFLOAT2 HDEngine::InputSystem::GetMousePosition()
{
	POINT mousePosition;
	GetCursorPos(&mousePosition);
	ScreenToClient(HODOengine::Instance().GetHWND(), &mousePosition);
	return HDMath::HDFLOAT2(mousePosition.x, mousePosition.y);
}

HDMath::HDFLOAT2 HDEngine::InputSystem::GetMousePositionNormalized()
{
	HDMath::HDFLOAT2 ret = GetMousePosition();
	RECT rect;
	GetClientRect(HODOengine::Instance().GetHWND(), &rect);
	ret.x /= static_cast<float>(rect.right) - rect.left;
	ret.y /= static_cast<float>(rect.bottom) - rect.top;
	return ret;
}
