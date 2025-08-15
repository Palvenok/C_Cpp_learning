#ifndef LABEL_H
#define LABEL_H

#include "control.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

class Label : public Control {
public:
    Label(int x, int y, const std::string& text, TTF_Font* font = nullptr);
    virtual ~Label();

    void Draw(SDL_Renderer* renderer) override;
    
    void SetText(const std::string& newText);
    std::string GetText() const;
    
    void SetColor(SDL_Color color);
    void SetFont(TTF_Font* font);

private:
    void CreateTexture(SDL_Renderer* renderer);
    void FreeTexture();
    
    std::string text;
    SDL_Texture* texture;
    TTF_Font* font;
    SDL_Color color;
};

#endif // LABEL_H