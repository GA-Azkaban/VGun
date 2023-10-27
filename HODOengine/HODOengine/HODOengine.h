#pragma once
#include "dllExporter.h"
#include "IHODOengine.h"

class HODOengine final : public IHODOengine
{
public:
	HODOengine();
	~HODOengine();

public:
	void Initialize() override;
	void Loop() override;
	void Finalize() override;

private:
	void Run();

	ATOM WindowRegisterClass(HINSTANCE hInstance);
	BOOL CreateWindows(HINSTANCE hInstance);
	//void ChangeDisplayResolution(int width, int height);

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	HWND _hWnd;
	MSG _msg;
	int _screenWidth;
	int _screenHeight;
	const WCHAR* _appName;

};
