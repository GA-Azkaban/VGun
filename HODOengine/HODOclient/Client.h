#pragma once
#ifdef _DEBUG
#pragma comment(lib, "..\\Build\\Debug\\HODOengine.lib")
//#define ENGINE_PATH L"..\\x64\\Debug\\RocketEngine.dll"
#else
#pragma comment(lib,"..\\Build\\Release\\HODOengine.lib")
//#define ENGINE_PATH L"Engine\\RocketEngine.dll" 
#endif // _DEBUG



// 엔진의 포인터를 받아오는...

class TestScene;

class Client
{
public:
	Client();
	~Client();

public:
	void Initialize();

private:

};
 
