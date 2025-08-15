#include "label.h"

Label::Label(int x, int y, const std::string& text, TTF_Font* font)
    : Control(x, y, 0, 0), text(text), texture(nullptr), font(font) {
    color = {0, 0, 0, 255}; // Черный цвет по умолчанию
}

Label::~Label() {
    FreeTexture();
}

void Label::SetText(const std::string& newText) {
    if (text != newText) {
        text = newText;
        FreeTexture();
    }
}

std::string Label::GetText() const {
    return text;
}

void Label::SetColor(SDL_Color newColor) {
    color = newColor;
    FreeTexture();
}

void Label::SetFont(TTF_Font* newFont) {
    if (newFont) {
        font = newFont;
        FreeTexture();
    }
}

void Label::CreateTexture(SDL_Renderer* renderer) {
    if (texture || text.empty() || !font) return;
    
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), text.length(), color);
    if (!surface) return;
    
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture) {
        width = surface->w;
        height = surface->h;
    }
    
    SDL_DestroySurface(surface);
}

void Label::FreeTexture() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

void Label::Draw(SDL_Renderer* renderer) {
    if (!isVisible || text.empty()) return;
    
    if (!texture) {
        CreateTexture(renderer);
    }
    
    if (texture) {
        SDL_FRect destRect = { (float)x, (float)y, (float)width, (float)height };
        SDL_RenderTexture(renderer, texture, nullptr, &destRect);
    }
}