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
    SDL_GameController *controller;
    
    //Creating a window
    if(SDL_CreateWindowAndRenderer(640, 480, SDL_WINDOW_OPENGL, &window, &renderer)){
        SDL_Log("Unable to create window and renderer: %s\n", SDL_GetError());
    }
    
    //The number of joysticks detected by SDL
    int joysticks = SDL_NumJoysticks();
    for(int joystickIndex = 0; joystickIndex < joysticks; ++joystickIndex){
        if(!SDL_IsGameController(joystickIndex)){
            continue;
        }
        cout << "Joystick at index: " << joystickIndex << " is a valid game controller\n";
        controller = SDL_GameControllerOpen(joystickIndex);
        break;
    }
    SDL_Delay(3000);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_GameControllerClose(controller);
    SDL_Quit();
    return 0;
}