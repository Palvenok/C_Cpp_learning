#pragma once

#include <Windows.h>

typedef NTSTATUS(WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);

class Utils
{
public:
    static DWORD GetOSVersion();
    static void ShowError(LPCSTR);
};