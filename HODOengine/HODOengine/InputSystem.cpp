#include "InputSystem.h"
#include "HODOengine.h"
#include <windows.h>

void HDEngine::InputSystem::Initialize(HWND hWnd, int screenWidth, int screenHeight)
{
	_hWnd = (HWND)hWnd;
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;

	_widthOffset = (screenWidth - 1920) / 2;
	_heightOffset = 0;

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
		_currentKeyState[keyCode] = GetAsyncKeyState(keyCode);
	}

	// 마우스 체크
	POINT mousePos;
	GetCursorPos(&mousePos);
	_mousePoint = mousePos;
	ScreenToClient(_hWnd, &mousePos);
	_currentMousePosition = { static_cast<float>(mousePos.x - _widthOffset), static_cast<float>(mousePos.y - _heightOffset) };
}

void HDEngine::InputSystem::Flush()
{
	if (_hWnd != GetFocus())
	{
		return;
	}

	for (int i = 0; i < 0xB0; i++)
	{
		_previousKeyState[i] = _currentKeyState[i];
		_currentKeyState[i] = false;
	}

	_previousMousePosition = _currentMousePosition;

	RECT windowRect;
	GetWindowRect(_hWnd, &windowRect);

	POINT mousePoint;

	LONG x = 0;
	LONG y = 0;

	/// 마우스 위치 이동 방식
	if (windowRect.right - 1 <= _mousePoint.x)
	{
		x = windowRect.left + 2;
		y = _mousePoint.y;
		mousePoint = { x, y };
		ScreenToClient(_hWnd, &mousePoint);
		_previousMousePosition = { static_cast<float>(mousePoint.x - _widthOffset), static_cast<float>(mousePoint.y - _heightOffset) };
		mouse_event(MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE, x * 65535 / GetSystemMetrics(SM_CXSCREEN), y * 65535 / GetSystemMetrics(SM_CYSCREEN), 0, 0);
	}
	else if (_mousePoint.x <= windowRect.left + 1)
	{
		x = windowRect.right - 2;
		y = _mousePoint.y;
		mousePoint = { x, y };
		ScreenToClient(_hWnd, &mousePoint);
		_previousMousePosition = { static_cast<float>(mousePoint.x - _widthOffset), static_cast<float>(mousePoint.y - _heightOffset) };
		mouse_event(MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE, x * 65535 / GetSystemMetrics(SM_CXSCREEN), y * 65535 / GetSystemMetrics(SM_CYSCREEN), 0, 0);
	}
	if (windowRect.bottom - 1 <= _mousePoint.y)
	{
		x = _mousePoint.x;
		y = windowRect.top + 2;
		mousePoint = { x, y };
		ScreenToClient(_hWnd, &mousePoint);
		_previousMousePosition = { static_cast<float>(mousePoint.x - _widthOffset), static_cast<float>(mousePoint.y - _heightOffset) };
		mouse_event(MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE, x * 65535 / GetSystemMetrics(SM_CXSCREEN), y * 65535 / GetSystemMetrics(SM_CYSCREEN), 0, 0);
	}
	else if (_mousePoint.y <= windowRect.top + 1)
	{
		x = _mousePoint.x;
		y = windowRect.bottom - 2;
		mousePoint = { x, y };
		ScreenToClient(_hWnd, &mousePoint);
		_previousMousePosition = { static_cast<float>(mousePoint.x - _widthOffset), static_cast<float>(mousePoint.y - _heightOffset) };
		mouse_event(MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE, x * 65535 / GetSystemMetrics(SM_CXSCREEN), y * 65535 / GetSystemMetrics(SM_CYSCREEN), 0, 0);
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
	return _currentMousePosition;
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
