#include "button.h"
#include <SDL3/SDL.h>

Button::Button(int x, int y, int width, int height, const std::string& text)
    : Control(x, y, width, height), text(text), isPressed(false) {}

void Button::Draw(SDL_Renderer* renderer) {
    if (!isVisible) return;
    
    SDL_SetRenderDrawColor(renderer, isPressed ? 150 : 200, 200, 200, 255);
    SDL_FRect rect = { (float)x, (float)y, (float)width, (float)height };
    SDL_RenderFillRect(renderer, &rect);
    
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderRect(renderer, &rect);
    
    // Здесь должна быть отрисовка текста с использованием SDL_ttf
}

void Button::HandleEvent(const SDL_Event& event) {
    if (!isVisible) return;
    
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        if (event.button.x >= x && event.button.x <= x + width &&
            event.button.y >= y && event.button.y <= y + height) {
            isPressed = true;
        }
    }
    else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
        if (isPressed && OnClick) {
            OnClick();
        }
        isPressed = false;
    }
}