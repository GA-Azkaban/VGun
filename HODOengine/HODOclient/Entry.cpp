#include "targetver.h"
#include <windows.h>

#include "Application.h"

///
/// 231026 오수안
/// 프로그램의 시작점
/// 

WCHAR g_ipAddress[256] = L"172.16.1.13";
int g_port = 7777;

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
