#include <iostream>
#include "SDL2/SDL.h"

using namespace std;

int main(int argc, char *argv[]){
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0){
        SDL_Log("Unable to initialize SDL: %s\n", SDL_GetError());
    }
    SDL_GameController *controller = NULL;
    for(int i = 0; i < SDL_NumJoysticks(); ++i){
        if(SDL_IsGameController(i)){
            controller = SDL_GameControllerOpen(i);
            if(controller){
                break;
            }else{
                cerr << "Could not open game controller " << i << '\n';
            }
        }
    }
    SDL_GameControllerGetBindForButton(controller, SDL_CONTROLLER_BUTTON_A);
    
    SDL_Window *myWindow;
        myWindow = SDL_CreateWindow("My window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
    while(1){
        SDL_Event event;
        SDL_PollEvent(&event);
        if(event.type == SDL_MOUSEBUTTONDOWN){
            cout << "Goodbye from the mouse\n";
            break;
        }else if(event.type == SDL_CONTROLLERBUTTONDOWN){
            cout << "Goodbye from the controller\n";
            break;
        }
    }
    SDL_Delay(3000);
    SDL_DestroyWindow(myWindow);
    SDL_GameControllerClose(controller);
    SDL_Quit();
    return 0;
}