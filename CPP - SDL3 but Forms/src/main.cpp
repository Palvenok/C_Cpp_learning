#include "form.h"
#include "button.h"
#include "label.h"
#include <SDL3/SDL.h>

int main(int argc, char* argv[]) {
    Form mainForm("My SDL3 Form", 800, 600);
    
    // Создаем кнопку
    Button* btn = new Button(50, 100, 100, 40, "Click me!");
    btn->OnClick = []() {
        SDL_Log("Button clicked!");
    };
    
    // Создаем метку
    Label* lbl = new Label(50, 50, "Hello, SDL3!", mainForm.defaultFont);
    lbl->SetColor({255, 0, 0, 255}); // Красный текст
    
    mainForm.AddControl(lbl);
    mainForm.AddControl(btn);
    
    mainForm.OnLoad = [&]() {
        SDL_Log("Form loaded");
    };
    
    mainForm.OnResize = [&](int w, int h) {
        SDL_Log("Window resized to %dx%d", w, h);
    };
    
    mainForm.Run();
    
    return 0;
}