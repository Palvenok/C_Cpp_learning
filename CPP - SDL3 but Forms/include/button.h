#ifndef BUTTON_H
#define BUTTON_H

#include "control.h"
#include <string>

class Button : public Control {
public:
    Button(int x, int y, int width, int height, const std::string& text);
    
    void Draw(SDL_Renderer* renderer) override;
    void HandleEvent(const SDL_Event& event) override;

    std::function<void()> OnClick;

private:
    std::string text;
    bool isPressed;
};

#endif // BUTTON_H