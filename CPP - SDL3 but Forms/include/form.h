#ifndef FORM_H
#define FORM_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <vector>
#include <functional>
#include "control.h"

class Form {
public:
    Form(const std::string& title, int width, int height);
    virtual ~Form();

    void Run();
    void AddControl(Control* control);

    // События формы
    std::function<void()> OnLoad;
    std::function<void()> OnClosed;
    std::function<void(int, int)> OnResize;

    TTF_Font* defaultFont;

protected:
    virtual void HandleEvents();
    virtual void Update();
    virtual void Render();

    std::string title;
    int width;
    int height;
    bool isRunning;
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::vector<Control*> controls;
};

#endif // FORM_H