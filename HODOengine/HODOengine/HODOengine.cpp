// HODOengine.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include <windows.h>
#include "HODOengine.h"
#include "SceneSystem.h"
#include "Scene.h"
#include "GameObject.h"

#include "SceneSystem.h"
#include "ObjectSystem.h"
#include "TimeSystem.h"
#include "InputSystem.h"
#include "RenderSystem.h"

HODOengine* HODOengine::_instance = nullptr;

IHODOengine* CreateEngine()
{
	HODOengine::_instance = new HODOengine();
	return HODOengine::_instance;
}

void ReleaseEngine(IHODOengine* instance)
{
	delete instance;
}

HODOengine::HODOengine()
	:_appName(L"HODO"),
	_sceneSystem(HDEngine::SceneSystem::Instance()),
	_objectSystem(HDEngine::ObjectSystem::Instance()),
	_timeSystem(HDEngine::TimeSystem::Instance()),
	_inputSystem(HDEngine::InputSystem::Instance()),
	_renderSystem(HDEngine::RenderSystem::Instance())
{

}

HODOengine::~HODOengine()
{

}

HODOengine& HODOengine::Instance()
{
	if (_instance == nullptr)
	{
		_instance = new HODOengine();
	}
	return *_instance;
}

void HODOengine::Initialize()
{
	HINSTANCE ins = GetModuleHandle(NULL);
	WindowRegisterClass(ins);
	CreateWindows(ins);


	_timeSystem.Initialize();
	_inputSystem.Initialize();
	_renderSystem.Initialize(_hWnd, _screenWidth, _screenHeight);
}

void HODOengine::Loop()
{
	while (1)
	{
		if (PeekMessage(&_msg, NULL, 0, 0, PM_REMOVE))
		{
			if (_msg.message == WM_QUIT) break;

			DispatchMessage(&_msg);
		}
		else
		{
			Run();
		}
	}

	// 엔진의 동작이 종료되면 instance도 nullptr로 초기화
	_instance = nullptr;
}

void HODOengine::Finalize()
{

}

HWND HODOengine::GetHWND()
{
	return _hWnd;
}


void HODOengine::Run()
{
	// Time Update
	_timeSystem.Update();

	// Input Update
	HDEngine::InputSystem::Instance().Update();

	// Destroy List -> GameObject OnDestroy, Clear
	for (auto destroyObj : HDEngine::SceneSystem::Instance().GetCurrentScene()->GetDestroyObjectList())
	{
		for (auto component : destroyObj->GetAllComponents())
		{
			component->OnDestroy();
		}
		HDEngine::SceneSystem::Instance().GetCurrentScene()->GetGameObjectList().erase(destroyObj);
	}
	HDEngine::SceneSystem::Instance().GetCurrentScene()->GetDestroyObjectList().clear();

	// Update Components
	for (auto gameObj : HDEngine::SceneSystem::Instance().GetCurrentScene()->GetGameObjectList())
	{
		for (auto component : gameObj->GetAllComponents())
		{
			component->Update();
		}
	}
	// Invoke Collision Events

	_renderSystem.DrawProcess();
}

ATOM HODOengine::WindowRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
    wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"HODO";
    wcex.hIconSm = NULL;

	return RegisterClassExW(&wcex);
}

BOOL HODOengine::CreateWindows(HINSTANCE hInstance)
{
	_hWnd = CreateWindowW(_appName, _appName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!_hWnd)
	{
		return FALSE;
	}

	RECT rect;

	GetClientRect(_hWnd, &rect);

	_screenWidth = rect.right - rect.left;
	_screenHeight = rect.bottom - rect.top;

	ShowWindow(_hWnd, SW_SHOWNORMAL);
	UpdateWindow(_hWnd);

	return TRUE;
}

LRESULT CALLBACK HODOengine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
