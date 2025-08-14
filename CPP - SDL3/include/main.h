#ifndef MAIN_CPP
#define MAIN_CPP

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <string>

constexpr int kScreenWidth{ 640 };
constexpr int kScreenHeight{ 480 };

bool init();
bool loadMedia();
void close();

SDL_Window* gWindow{ nullptr };
SDL_Surface* gScreenSurface{ nullptr };
SDL_Surface* gHelloWorld{ nullptr };


#endif