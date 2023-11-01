#include "targetver.h"
#include <windows.h>

#include "Application.h"

///
/// 231026 오수안
/// 프로그램의 시작점
/// 

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    Application app;
    app.Run();
}