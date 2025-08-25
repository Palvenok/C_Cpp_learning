#include <main.h>

int main() 
{
    Logger::initialized();
    HINSTANCE hInstance = GetModuleHandle(nullptr);
    int nCmdShow = SW_SHOWDEFAULT;
    return wWinMain(hInstance, nullptr, nullptr, nCmdShow);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow) 
{   
    const wchar_t CLASS_NAME[] = L"Main window";
    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClassW(&wc);

    HWND hwnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"Title",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 600, 400,
        nullptr, nullptr, hInstance, nullptr
    );

    if (hwnd == nullptr) 
    {
        LOG_ERROR("hwnd creating failure!");
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessageW(&msg, nullptr, 0, 0)) 
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
    switch (uMsg) 
    {
        
        case WM_CLOSE:
            if (MessageBoxW(hwnd, L"Really quit?", L"My application", MB_OKCANCEL) == IDOK)
                DestroyWindow(hwnd);
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_PAINT:
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));

            const wchar_t* text = L"Привет, мир!";
            TextOutW(hdc, 50, 50, text, lstrlenW(text));

            EndPaint(hwnd, &ps);
            return 0;

    }
    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}