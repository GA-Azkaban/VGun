#pragma once
#include "dllExporter.h"
#include "IHODOengine.h"

namespace HDEngine
{
	class SceneSystem;
	class ObjectSystem;
	class InputSystem;
	class TimeSystem;
	class IDSystem;
	class RenderSystem;
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
	HDEngine::SceneSystem& _sceneSystem;
	HDEngine::ObjectSystem& _objectSystem;
	HDEngine::TimeSystem& _timeSystem;
	HDEngine::InputSystem& _inputSystem;
	HDEngine::RenderSystem& _renderSystem;
};
