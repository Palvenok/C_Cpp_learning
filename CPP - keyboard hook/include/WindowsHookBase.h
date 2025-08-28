#pragma once

#include <windows.h>

template<typename Derived>
class WindowsHookBase {
protected:
    HHOOK hHook = nullptr;
    DWORD threadId = 0;
    int hookType = 0;

    static LRESULT CALLBACK HookProcStatic(int code, WPARAM wParam, LPARAM lParam) 
    {
        return Derived::GetInstance()->HookCallback(code, wParam, lParam);
    }

    LRESULT HookCallback(int code, WPARAM wParam, LPARAM lParam) 
    {
        return static_cast<Derived*>(this)->HookCallback(code, wParam, lParam);
    }

public:
    bool Install(int type, DWORD thread = 0) 
    {
        hookType = type;
        threadId = thread;
        hHook = SetWindowsHookEx(hookType, HookProcStatic, GetModuleHandle(nullptr), threadId);
        return (hHook != nullptr);
    }

    void Uninstall() 
    {
        if (hHook) 
        {
            UnhookWindowsHookEx(hHook);
            hHook = nullptr;
        }
    }

    ~WindowsHookBase() {
        Uninstall();
    }
};


