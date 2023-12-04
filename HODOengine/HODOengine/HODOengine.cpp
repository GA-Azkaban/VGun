// HODOengine.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include <windows.h>
#include "HODOengine.h"
#include "GraphicsRenderer.h"
#include "SceneSystem.h"
#include "Scene.h"
#include "GameObject.h"

#include "SceneSystem.h"
#include "ObjectSystem.h"
#include "TimeSystem.h"
#include "InputSystem.h"

IHODOengine* CreateEngine()
{
	return new HODOengine();
}

void ReleaseEngine(IHODOengine* instance)
{
	delete instance;
}

HODOengine::HODOengine()
	:_appName(L"HODO"),
	_sceneSystem(hodoEngine::SceneSystem::Instance()),
	_objectSystem(hodoEngine::ObjectSystem::Instance()),
	_timeSystem(hodoEngine::TimeSystem::Instance()),
	_inputSystem(hodoEngine::InputSystem::Instance())
{

}

HODOengine::~HODOengine()
{

}

void HODOengine::Initialize()
{
	HINSTANCE ins = GetModuleHandle(NULL);
	WindowRegisterClass(ins);
	CreateWindows(ins);

	hodoEngine::GraphicsRenderer::Instance().LoadGraphicsDll(L"MZDX11Renderer.dll");
	hodoEngine::GraphicsRenderer::Instance().SetOutputWindow(_hWnd);

	_timeSystem.Initialize();
	_inputSystem.Initialize((int)_hWnd, _screenWidth, _screenHeight);
}

void HODOengine::Loop()
{
	// 프로세스 내에서 하나의 엔진 인스턴스만 돌고 있을 수 있도록 함
	static HODOengine* _instance;

	if (_instance == nullptr)
	{
		_instance = this;
	}

	if (_instance != this)
	{
		return;
	}

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

void HODOengine::Run()
{
	// Time Update
	_timeSystem.Update();
	// Destroy List -> GameObject OnDestroy, Clear
	for (auto destroyObj : hodoEngine::SceneSystem::Instance().GetCurrentScene()->GetDestroyObjectList())
	{
		for (auto component : destroyObj->GetAllComponents())
		{
			component->OnDestroy();
		}
		hodoEngine::SceneSystem::Instance().GetCurrentScene()->GetGameObjectList().erase(destroyObj);
	}
	hodoEngine::SceneSystem::Instance().GetCurrentScene()->GetDestroyObjectList().clear();

	// Update Components
	for (auto gameObj : hodoEngine::SceneSystem::Instance().GetCurrentScene()->GetGameObjectList())
	{
		for (auto component : gameObj->GetAllComponents())
		{
			component->Update();
		}
	}
	// Invoke Collision Events
	// Renderer Update
	hodoEngine::GraphicsRenderer::Instance().Update(hodoEngine::TimeSystem::Instance().GetDeltaTime());
	// Renderer Render
	hodoEngine::GraphicsRenderer::Instance().Render();
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
