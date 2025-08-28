#include <Windows.h>
#include "Settings.h"
#include "Utils.h"
#include "KeyboardHook.h"


int main(int argc, char** argv)
{
	Settings settings = { .popup = false };

	if (argc > 1 && strcmp(argv[1], "nopopup") == 0)
		settings.popup = false;
	else
		settings.popup = Utils::GetOSVersion() >= 10;

	HANDLE hMutex = CreateMutex(0, 0, "Switchy");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		Utils::ShowError("Another instance of Switchy is already running!");
		return 1;
	}

	KeyboardHook kh(settings);

	if(!kh.InstallKeyboard())
	{
		Utils::ShowError("Keyboard hool install error!");
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