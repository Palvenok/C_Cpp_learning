#pragma once

#include <Windows.h>
#include <functional>

typedef NTSTATUS(WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);

class Utils
{
public:
    static DWORD GetOSVersion();
    static void ShowError(LPCSTR);
};