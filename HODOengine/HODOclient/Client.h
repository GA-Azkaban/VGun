#pragma once
#ifdef _DEBUG
#pragma comment(lib, "..\\x64\\Debug\\HODOengine.lib")
//#define ENGINE_PATH L"..\\x64\\Debug\\RocketEngine.dll"
#else
//#pragma comment(lib,"..\\x64\\Release\\HODOengine.lib")
//#define ENGINE_PATH L"Engine\\RocketEngine.dll" 
#endif // _DEBUG



// ������ �����͸� �޾ƿ���...

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

