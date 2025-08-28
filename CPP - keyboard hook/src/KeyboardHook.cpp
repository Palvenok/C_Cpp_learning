#include "KeyboardHook.h"

KeyboardHook* KeyboardHook::instance = nullptr;

LRESULT KeyboardHook::HookCallback(int code, WPARAM wParam, LPARAM lParam) 
{
    if (code >= 0) 
	{
        auto* kb = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
        bool down = (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN);
        OnKey.Invoke(kb->vkCode, down);
    }
    return CallNextHookEx(nullptr, code, wParam, lParam);

}