#ifndef CONTROL_H
#define CONTROL_H

#include <SDL3/SDL.h>
#include <functional>

class Control {
public:
    Control(int x, int y, int width, int height);
    virtual ~Control();

    virtual void HandleEvent(const SDL_Event& event);
    virtual void Update();
    virtual void Draw(SDL_Renderer* renderer);

    void SetVisible(bool visible);
    bool GetVisible() const;
    
    virtual void SetPosition(int newX, int newY);
    virtual void SetSize(int newWidth, int newHeight);
    
    int GetX() const { return x; }
    int GetY() const { return y; }
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }

protected:
    int x, y, width, height;
    bool isVisible;
};

#endif // CONTROL_H