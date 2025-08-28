#include <Windows.h>
#include "Utils.h"
#include "KeyboardHook.h"
#include <iostream>

void HandleKey(DWORD, bool);

int main(int argc, char** argv)
{
	KeyboardHook hook;

    hook.OnKey += HandleKey;						//First subscriber - print pressed/released key
	hook.OnKey += [](DWORD vk, bool is_pressed)		//Second subscriber - exit from app
	{
		if (vk == VK_ESCAPE && is_pressed) {
			std::cout << "Exit...\n";
			PostQuitMessage(0);
		}
	};

    if (!hook.InstallKeyboardHook()) 
	{
        std::cerr << "Hook not installed!\n";
        return 1;
    }

	MSG messages;
	while (GetMessage(&messages, nullptr, 0, 0))
	{
		TranslateMessage(&messages);
		DispatchMessage(&messages);
	}
	return 0;
}

void HandleKey(DWORD vk, bool is_pressed)
{
	if (is_pressed) 
		std::cout << "Pressed: " << vk << "\n";
	else 
		std::cout << "Released: " << vk << "\n";
}