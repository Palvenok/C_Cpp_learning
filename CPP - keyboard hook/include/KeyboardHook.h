#pragma once

#include <Windows.h>
#include "WindowsHookBase.h"
#include "Utils.h"
#include "Actions.h"

class KeyboardHook : public WindowsHookBase<KeyboardHook>
{
protected:
    static KeyboardHook* instance;
public:

    Action<DWORD, bool> OnKey;

    KeyboardHook() 
    {
        if(instance != nullptr) return;
        instance = this; 
    }

    static KeyboardHook* GetInstance() { return instance; }

    LRESULT HookCallback(int code, WPARAM wParam, LPARAM lParam);

    bool InstallKeyboardHook(DWORD thread = 0)
    {
        return Install(WH_KEYBOARD_LL, thread);
    }

    ~KeyboardHook()
    {
        OnKey.Clear();
    }
};
