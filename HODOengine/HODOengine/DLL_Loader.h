#pragma once
#include <windows.h>
#include <string>

#ifdef _DEBUG
#define GRAPHICSDLL_PATH (L"..\\x64\\Debug\\RocketDX11.dll") // (".\\my\\Path\\"#filename) ".\\my\\Path\\filename"
#else
#define GRAPHICSDLL_PATH ("Graphics\\RocketDX11.dll"#filename)
#endif // _DEBUG

// TODO : DLL �ε��ؼ� ���⼭ ����ְ� ���� �ֳ��ϸ�..? �ƴѰ�?.. ��ü���� ����ִٰ� �Ű������� ��������ϳ�..?

namespace HDEngine
{
	class DLL_Loader
	{
	public:
		DLL_Loader();
		~DLL_Loader();
		void LoadDLL(std::wstring dllPath);
		HMODULE GetDLLHandle();

	private:
		HMODULE _dllHandle;
	};
}
