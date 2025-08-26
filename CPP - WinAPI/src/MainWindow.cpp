#include "MainWindow.h"

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(m_hwnd, &ps);

            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));
            
            const wchar_t* text = L"Привет, мир!";
            TextOutW(hdc, 50, 50, text, lstrlenW(text));

            EndPaint(m_hwnd, &ps);
        }
        return 0;

    default:
        return DefWindowProcW(m_hwnd, uMsg, wParam, lParam);
    }
    return TRUE;
}