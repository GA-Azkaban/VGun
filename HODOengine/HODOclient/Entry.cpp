#include "targetver.h"
#include <windows.h>

#include "Application.h"

///
/// 231026 오수안
/// 프로그램의 시작점
/// 

WCHAR g_ipAddress[256] = L"14.35.98.143";
//int g_port = 7776;		// 짧은거
int g_port = 7777;	// 60초
//int g_port = 7778;	// 더 긴거
//int g_port = 7779;

void ParseCmdLine(LPWSTR lpCmdLine)
{
	int argc;
	LPWSTR* argv = CommandLineToArgvW(lpCmdLine, &argc);

	if (argv == nullptr || argc < 2)
		return;

	wcscpy_s(g_ipAddress, argv[0]);
	g_port = _wtoi(argv[1]);

	LocalFree(argv);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	ParseCmdLine(lpCmdLine);
    Application app;
    app.Run();
}
