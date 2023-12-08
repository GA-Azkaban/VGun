#include "InputSystem.h"
#include "HODOengine.h"
#include <windows.h>


void hodoEngine::InputSystem::Initialize()
{
	for (int keyCode = 0; keyCode < 256; ++keyCode)
	{
		_previousKeyState[keyCode] = false;
		_currentKeyState[keyCode] = false;
	}
}

void hodoEngine::InputSystem::Update()
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

HDMaths::HDFLOAT2 hodoEngine::InputSystem::GetMousePosition()
{
	POINT mousePosition;
	GetCursorPos(&mousePosition);
	ScreenToClient(HODOengine::Instance().GetHWND(), &mousePosition);
	return HDMaths::HDFLOAT2(mousePosition.x, mousePosition.y);
}

HDMaths::HDFLOAT2 hodoEngine::InputSystem::GetMousePositionNormalized()
{
	HDMaths::HDFLOAT2 ret = GetMousePosition();
	RECT rect;
	GetClientRect(HODOengine::Instance().GetHWND(), &rect);
	ret.x /= static_cast<float>(rect.right) - rect.left;
	ret.y /= static_cast<float>(rect.bottom) - rect.top;
	return ret;
}
