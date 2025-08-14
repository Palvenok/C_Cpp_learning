#include "main.h"

int main( int argc, char* args[] )
{
    if(!init())
    {
        SDL_Log( "Unable to initialize program!\n" );
        close();
        return 1;
    }
    
    if(!loadMedia())
    {
        SDL_Log( "Unable to load media!\n" );
        close();
        return 2;
    }

    bool quit{ false };

    SDL_Event e;
    SDL_zero(e);
    
    while( !quit )
    {
        while( SDL_PollEvent( &e ) )
            if( e.type == SDL_EVENT_QUIT )
                quit = true;
        
        SDL_FillSurfaceRect( gScreenSurface, nullptr, SDL_MapSurfaceRGB( gScreenSurface, 0xFF, 0xFF, 0xFF ) );
        SDL_BlitSurface( gHelloWorld, nullptr, gScreenSurface, nullptr );
        SDL_UpdateWindowSurface( gWindow );
    }
    
    close();
    return 0;
}


bool init()
{
    bool success{ true };

    if(!SDL_Init( SDL_INIT_VIDEO ))
    {
        SDL_Log( "SDL could not initialize! SDL error: %s\n", SDL_GetError() );
        return false;
    }

    if( gWindow = SDL_CreateWindow( "App", kScreenWidth, kScreenHeight, 0 ); gWindow == nullptr )
    {
        SDL_Log( "Window could not be created! SDL error: %s\n", SDL_GetError() );
        return false;
    }

    gScreenSurface = SDL_GetWindowSurface( gWindow );

    return true;
}

bool loadMedia()
{
    std::string imagePath{ "assets/img.bmp" };
    if( gHelloWorld = SDL_LoadBMP( imagePath.c_str() ); gHelloWorld == nullptr )
    {
        SDL_Log( "Unable to load image %s! SDL Error: %s\n", imagePath.c_str(), SDL_GetError() );
        return false;
    }

    return true;
}

void close()
{
    SDL_DestroySurface( gHelloWorld );
    gHelloWorld = nullptr;
    
    SDL_DestroyWindow( gWindow );
    gWindow = nullptr;
    gScreenSurface = nullptr;

    SDL_Quit();
}