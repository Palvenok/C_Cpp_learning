#pragma once

#include <Windows.h>
#include "WindowsHookBase.h"
#include "Settings.h"

class KeyboardHook : public WindowsHookBase<KeyboardHook>
{
protected:
    static KeyboardHook* instance;

    Settings settings = { .popup = false };

    bool enabled = true;
    bool keystrokeCapsProcessed = false;
    bool keystrokeShiftProcessed = false;
    bool winPressed = false;

    void PressKey(int keyCode);
    void ReleaseKey(int keyCode);
    void ToggleCapsLockState();

    
public:

    KeyboardHook() 
    {
        if(instance != nullptr) return;
        instance = this; 
    }

    KeyboardHook(Settings& settings) 
    { 
        if(instance != nullptr) return;
        this->settings = settings;
        instance = this; 
    }

    static KeyboardHook* GetInstance() { return instance; }
    
    LRESULT HookCallback(int code, WPARAM wParam, LPARAM lParam);

    bool InstallKeyboard(DWORD thread = 0)
    {
        return Install(WH_KEYBOARD_LL, thread);
    }

    void UpdateSettings(Settings& settings)
    {
        this->settings = settings;
    }
};
