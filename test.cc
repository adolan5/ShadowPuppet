#include <iostream>
#include "SDL.h"

using namespace std;

#define MAX_CONTROLLERS 4

int main(int argc, char *argv[]){
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0){
        SDL_Log("Unable to initialize SDL: %s\n", SDL_GetError());
    }
    
    SDL_Window *myWindow;
    myWindow = SDL_CreateWindow("My window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    //An array of pointers to game controller structures
    SDL_GameController *controller;
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
    SDL_DestroyWindow(myWindow);
    SDL_GameControllerClose(controller);
    SDL_Quit();
    return 0;
}