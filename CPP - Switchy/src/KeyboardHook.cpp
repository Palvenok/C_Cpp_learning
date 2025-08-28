#include "KeyboardHook.h"

KeyboardHook* KeyboardHook::instance = nullptr;

LRESULT KeyboardHook::HookCallback(int code, WPARAM wParam, LPARAM lParam) 
{
    KBDLLHOOKSTRUCT* key = (KBDLLHOOKSTRUCT*)lParam;
	if (code == HC_ACTION && !(key->flags & LLKHF_INJECTED))
	{
		if (key->vkCode == VK_CAPITAL)
		{
			if (wParam == WM_SYSKEYDOWN && !keystrokeCapsProcessed)
			{
				keystrokeCapsProcessed = true;
				enabled = !enabled;
				return 1;
			}

			if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
			{
				keystrokeCapsProcessed = false;

				if (winPressed)
				{
					winPressed = false;
					ReleaseKey(VK_LWIN);
				}

				if (enabled && !settings.popup)
				{
					if (!keystrokeShiftProcessed)
					{
						PressKey(VK_MENU);
						PressKey(VK_LSHIFT);
						ReleaseKey(VK_MENU);
						ReleaseKey(VK_LSHIFT);
					}
					else
					{
						keystrokeShiftProcessed = false;
					}
				}
			}

			if (!enabled)
			{
				return CallNextHookEx(hHook, code, wParam, lParam);
			}

			if (wParam == WM_KEYDOWN && !keystrokeCapsProcessed)
			{
				keystrokeCapsProcessed = true;

				if (keystrokeShiftProcessed == true)
				{
					ToggleCapsLockState();
					return 1;
				}
				else
				{
					if (settings.popup)
					{
						PressKey(VK_LWIN);
						PressKey(VK_SPACE);
						ReleaseKey(VK_SPACE);
						winPressed = true;
					}
				}
			}
			return 1;
		}

		else if (key->vkCode == VK_LSHIFT)
		{

			if ((wParam == WM_KEYUP || wParam == WM_SYSKEYUP) && !keystrokeCapsProcessed)
			{
				keystrokeShiftProcessed = false;
			}

			if (!enabled)
			{
				return CallNextHookEx(hHook, code, wParam, lParam);
			}

			if (wParam == WM_KEYDOWN && !keystrokeShiftProcessed)
			{
				keystrokeShiftProcessed = true;

				if (keystrokeCapsProcessed == true)
				{
					ToggleCapsLockState();
					if (settings.popup)
					{
						PressKey(VK_LWIN);
						PressKey(VK_SPACE);
						ReleaseKey(VK_SPACE);
						winPressed = true;
					}

					return 0;
				}
			}
			return 0;
		}
	}

	return CallNextHookEx(hHook, code, wParam, lParam);
}

void KeyboardHook::PressKey(int keyCode)
{
	keybd_event(keyCode, 0, 0, 0);
}

void KeyboardHook::ReleaseKey(int keyCode)
{
	keybd_event(keyCode, 0, KEYEVENTF_KEYUP, 0);
}

void KeyboardHook::ToggleCapsLockState()
{
	PressKey(VK_CAPITAL);
	ReleaseKey(VK_CAPITAL);
}