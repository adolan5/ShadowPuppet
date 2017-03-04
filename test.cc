#include <iostream>
#include "SDL.h"

using namespace std;

#define MAX_CONTROLLERS 4

int main(int argc, char *argv[]){
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0){
        SDL_Log("Unable to initialize SDL: %s\n", SDL_GetError());
    }
    
    //Pointers for our window and renderer, and controller
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *backgroundSurface;
    SDL_Texture *background;
    SDL_GameController *controller;
    //An event to check if we're still running
    SDL_Event event;
    
    //Creating a window
    if(SDL_CreateWindowAndRenderer(640, 480, SDL_WINDOW_OPENGL, &window, &renderer)){
        cerr << argv[0] << ": ";
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create window and renderer! Error: %s\n", SDL_GetError());
        return 1;
    }
    //Loading the background image as a surface
    backgroundSurface = SDL_LoadBMP("./images/wht-marble.bmp");
    if(!backgroundSurface){
        cerr << argv[0] << ": ";
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't load backgroundSurface from bmp! Error: %s\n", SDL_GetError());
        return 1;
    }
    //Creating a texture from the above surface
    background = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    if(!background){
        cerr << argv[0] << ": ";
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from background surface! Error: %s\n", SDL_GetError());
        return 1;
    }
    //Now we're done with the surface, so we can free it
    SDL_FreeSurface(backgroundSurface);
    
    //Opening the gamepad if one is connected
    bool gamepadConnected = false;
    int joysticks = SDL_NumJoysticks();
    for(int joystickIndex = 0; joystickIndex < joysticks; ++joystickIndex){
        if(!SDL_IsGameController(joystickIndex)){
            continue;
        }
        cout << "Joystick at index: " << joystickIndex << " is a valid game controller\n";
        controller = SDL_GameControllerOpen(joystickIndex);
        gamepadConnected = true;
        break;
    }
    
    while(1){
        SDL_PollEvent(&event);
        if(event.type == SDL_QUIT){
            break;
        }
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, background, NULL, NULL);
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    if(gamepadConnected){
        SDL_GameControllerClose(controller);
    }
    SDL_Quit();
    return 0;
}