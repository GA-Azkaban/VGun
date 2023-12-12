#pragma once
#include "dllExporter.h"
#include "IHODOengine.h"

namespace hodoEngine
{
	class SceneSystem;
	class ObjectSystem;
	class InputSystem;
	class TimeSystem;
	class IDSystem;
	class DebugSystem;
}

class HODOengine final : public IHODOengine
{
public:
	HODOengine();
	~HODOengine();

public:
	friend IHODOengine* CreateEngine();
	static HODOengine& Instance();
	void Initialize() override;
	void Loop() override;
	void Finalize() override;

	HWND GetHWND();

private:
	void Run();

	ATOM WindowRegisterClass(HINSTANCE hInstance);
	BOOL CreateWindows(HINSTANCE hInstance);
	//void ChangeDisplayResolution(int width, int height);

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	static HODOengine* _instance;
	HWND _hWnd;
	MSG _msg;
	int _screenWidth;
	int _screenHeight;
	const WCHAR* _appName;

private:
	hodoEngine::SceneSystem& _sceneSystem;
	hodoEngine::ObjectSystem& _objectSystem;
	hodoEngine::TimeSystem& _timeSystem;
	hodoEngine::InputSystem& _inputSystem;
	hodoEngine::DebugSystem& _debugSystem;
};
