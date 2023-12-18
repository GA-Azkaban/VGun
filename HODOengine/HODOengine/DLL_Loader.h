#pragma once
#include <windows.h>
#include <string>

#ifdef _DEBUG
#define GRAPHICSDLL_PATH (L"..\\x64\\Debug\\RocketDX11.dll") // (".\\my\\Path\\"#filename) ".\\my\\Path\\filename"
#else
#define GRAPHICSDLL_PATH ("Graphics\\RocketDX11.dll"#filename)
#endif // _DEBUG

// TODO : DLL 로드해서 여기서 들고있게 하자 왜냐하면..? 아닌가?.. 본체에서 들고있다가 매개변수로 나눠줘야하나..?

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
