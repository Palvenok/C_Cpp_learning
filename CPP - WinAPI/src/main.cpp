
#include <main.h>
#include "MainWindow.h"

int main() 
{
    //Logger::initialized();
    HINSTANCE hInstance = GetModuleHandle(nullptr);
    int nCmdShow = SW_SHOWDEFAULT;
    return wWinMain(hInstance, nullptr, nullptr, nCmdShow);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow) 
{   
    MainWindow win;

    if (!win.Create(L"Learn to Program Windows", WS_OVERLAPPEDWINDOW))
    {
        LOG_ERROR("Create window error!");
        return 0;
    }

    ShowWindow(win.Window(), nCmdShow);

    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}