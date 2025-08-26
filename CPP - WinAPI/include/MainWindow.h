#pragma once

#include "BaseWindow.h"

class MainWindow : public BaseWindow<MainWindow>
{
public:
    LPCWSTR ClassName() const { return L"Sample Window Class"; }
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};