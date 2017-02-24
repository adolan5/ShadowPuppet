#include <iostream>
#include "SDL.h"

using namespace std;

int main(int argc, char *argv[]){
    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        SDL_Log("Unable to initialize SDL: %s\n", SDL_GetError());
    }
    
    SDL_Window *myWindow;
    myWindow = SDL_CreateWindow("My window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
    SDL_Delay(3000);
    SDL_DestroyWindow(myWindow);
    SDL_Quit();
    return 0;
}