#include "form.h"
#include <SDL3/SDL.h>

Form::Form(const std::string& title, int width, int height) 
    : title(title), width(width), height(height), isRunning(false), defaultFont(nullptr) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return;
    }

    if (!TTF_Init()) {
        SDL_Log("Unable to initialize SDL_ttf: %s", SDL_GetError());
        return;
    }
    
    window = SDL_CreateWindow(title.c_str(), width, height, SDL_WINDOW_RESIZABLE);
    if (!window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return;
    }
    
    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return;
    }

    // Загрузка шрифта по умолчанию
    defaultFont = TTF_OpenFont("arial.ttf", 16);
    if (!defaultFont) {
        SDL_Log("Failed to load font: %s", SDL_GetError());
    }
}

Form::~Form() {
    for (auto control : controls) {
        delete control;
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    if (defaultFont) {
        TTF_CloseFont(defaultFont);
    }
    
    TTF_Quit();
    SDL_Quit();
}

void Form::Run() {
    isRunning = true;
    if (OnLoad) OnLoad();
    
    while (isRunning) {
        HandleEvents();
        Update();
        Render();
    }
}

void Form::AddControl(Control* control) {
    controls.push_back(control);
}

void Form::HandleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
                isRunning = false;
                if (OnClosed) OnClosed();
                break;
                
            case SDL_EVENT_WINDOW_RESIZED:
                width = event.window.data1;
                height = event.window.data2;
                if (OnResize) OnResize(width, height);
                break;
        }
        
        for (auto control : controls) {
            control->HandleEvent(event);
        }
    }
}

void Form::Update() {
    for (auto control : controls) {
        control->Update();
    }
}

void Form::Render() {
    SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
    SDL_RenderClear(renderer);
    
    for (auto control : controls) {
        control->Draw(renderer);
    }
    
    SDL_RenderPresent(renderer);
}