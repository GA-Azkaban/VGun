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
#include "DebugSystem.h"
#include "RenderSystem.h"
#include "PhysicsSystem.h"
#include "GraphicsObjFactory.h"
#include "EventSystem.h"
#include "SoundSystem.h"
#include "UISystem.h"
#include "TweenSystem.h"

#include "MaterialLoader.h"
#include "DLL_Loader.h"

#ifdef _DEBUG
#define GRAPHICSDLL_PATH (L"RocketDX11.dll") // (".\\my\\Path\\"#filename) ".\\my\\Path\\filename"
#else
#define GRAPHICSDLL_PATH (L"RocketDX11.dll")
#endif // _DEBUG

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
	_dllLoader(new HDEngine::DLL_Loader()),
	_sceneSystem(HDEngine::SceneSystem::Instance()),
	_objectSystem(HDEngine::ObjectSystem::Instance()),
	_timeSystem(HDEngine::TimeSystem::Instance()),
	_inputSystem(HDEngine::InputSystem::Instance()),
	_renderSystem(HDEngine::RenderSystem::Instance()),
	_debugSystem(HDEngine::DebugSystem::Instance()),
	_physicsSystem(HDEngine::PhysicsSystem::Instance()),
	_graphicsObjFactory(HDEngine::GraphicsObjFactory::Instance()),
	_eventSystem(HDEngine::EventSystem::Instance()),
	_soundSystem(HDEngine::SoundSystem::Instance()),
	_uiSystem(HDEngine::UISystem::Instance()),
	_materialLoader(HDEngine::MaterialLoader::Instance()),
	_tweenSystem(HDEngine::TweenSystem::Instance())
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
	_debugSystem.Initialize();
	
	HINSTANCE ins = GetModuleHandle(NULL);
	WindowRegisterClass(ins);
	CreateWindows(ins);
	_dllLoader->LoadDLL(GRAPHICSDLL_PATH);

	// 렌더 먼저 그다음에 인풋
	_graphicsObjFactory.Initialize(_dllLoader->GetDLLHandle());
	_renderSystem.Initialize(_hWnd, _dllLoader->GetDLLHandle(), _screenWidth, _screenHeight);
	_timeSystem.Initialize();
	_inputSystem.Initialize(_hWnd, ins);
	_physicsSystem.Initialize();
	_uiSystem.Initialize();
	_materialLoader.LoadMaterialData("materialData.json");
}

void HODOengine::Loop()
{
	_physicsSystem.PreparePhysics();

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
	_uiSystem.Finalize();
	_physicsSystem.Finalize();
	_renderSystem.Finalize();
	// _debugSystem.Finalize();
	// _inputSystem.Finalize();
	// _timeSystem.Finalize();
	_graphicsObjFactory.Finalize();
}

HWND HODOengine::GetHWND()
{
	return _hWnd;
}


void HODOengine::Quit()
{
	Finalize();
	DestroyWindow(_hWnd);
}

void HODOengine::ShowWindowCursor(bool show)
{
	ShowCursor(show);
}

void HODOengine::Run()
{
	_timeSystem.Update();
	_objectSystem.FlushDestroyObjectList();

	_inputSystem.Update();
	_debugSystem.Update();
	_uiSystem.Update();

	_objectSystem.UpdateCurrentSceneObjects();
	_soundSystem.Update();

	_objectSystem.LateUpdateCurrentSceneObjects();

	// draw
	_renderSystem.Update(_timeSystem.GetDeltaTime());
	_renderSystem.DrawProcess();

	_eventSystem.InvokeEvent();
	_objectSystem.UpdateDisableList();
	_objectSystem.UpdateEnableList();

	// physicsUpdate, temporary location
	_physicsSystem.Flush();
	_physicsSystem.Update();

	// refresh input for next frame
	_inputSystem.Flush();
	_tweenSystem.Update();

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
	DEVMODE dmSettings;									// Device Mode variable - Needed to change modes
	memset(&dmSettings, 0, sizeof(dmSettings));			// Makes Sure Memory's Cleared

	// Get the current display settings.  This function fills our the settings.
	if (!EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dmSettings))
	{
		// Display error message if we couldn't get display settings
		MessageBox(NULL, L"Could Not Enum Display Settings", L"Error", MB_OK);
		return FALSE;
	}

	_screenWidth = 2560;
	_screenHeight = 1440;
	//_screenWidth = 1920;
	//_screenHeight = 1080;
	dmSettings.dmPelsWidth = _screenWidth;
	dmSettings.dmPelsHeight = _screenHeight;
	dmSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;

	// This function actually changes the screen to full screen
	// CDS_FULLSCREEN Gets Rid Of Start Bar.
	// We always want to get a result from this function to check if we failed
	int result = ChangeDisplaySettings(&dmSettings, CDS_FULLSCREEN);
	dmSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
	// Check if we didn't receive a good return message From the function
	if (result != DISP_CHANGE_SUCCESSFUL)
	{
		// Display the error message and quit the program
		MessageBox(NULL, L"Display Mode Not Compatible", L"Error", MB_OK);
		PostQuitMessage(0);
	}
	
	_hWnd = CreateWindowW(_appName, _appName, WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		0, 0, _screenWidth, _screenHeight, nullptr, nullptr, hInstance, nullptr);

	if (!_hWnd)
	{
		return FALSE;
	}

	ShowWindow(_hWnd, SW_SHOWMAXIMIZED);
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
