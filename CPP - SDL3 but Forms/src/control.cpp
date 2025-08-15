#include "control.h"

Control::Control(int x, int y, int width, int height) 
    : x(x), y(y), width(width), height(height), isVisible(true) {}

Control::~Control() {}

void Control::HandleEvent(const SDL_Event& event) {
    // Базовая реализация обработки событий
}

void Control::Update() {
    // Базовая реализация обновления
}

void Control::Draw(SDL_Renderer* renderer) {
    if (!isVisible) return;
    // Базовая реализация отрисовки
}

void Control::SetVisible(bool visible) { isVisible = visible; }
bool Control::GetVisible() const { return isVisible; }

void Control::SetPosition(int newX, int newY) {
    x = newX;
    y = newY;
}

void Control::SetSize(int newWidth, int newHeight) {
    width = newWidth;
    height = newHeight;
}