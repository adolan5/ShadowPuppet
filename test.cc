#include <iostream>
#include "SDL.h"

using namespace std;

//Function definitions

int main(int argc, char *argv[]){
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0){
        SDL_Log("Unable to initialize SDL: %s\n", SDL_GetError());
    }
    
    //Pointers for our window and renderer, and controller
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_GameController *controller;
    //Pointers to our various surfaces and textures
    SDL_Surface *backgroundSurface;
    SDL_Texture *background;
    SDL_Surface *playerSurface;
    SDL_Texture *playerTexture;
    //An event to be polled
    SDL_Event event;
    bool gameRunning = true;
    
    //Player rect
    SDL_Rect player;
    player.x = 0;
    player.y = 0;
    player.w = 40;
    player.h = 40;
    
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
    SDL_FreeSurface(backgroundSurface);
    //Loading player surface, creating texture
    playerSurface = SDL_LoadBMP("./images/Smiley.bmp");
    if(!playerSurface){
        cerr << argv[0] << ": ";
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't load playerSurface from bmp! Error: %s\n", SDL_GetError());
        return 1;
    }
    playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
    if(!playerTexture){
        cerr << argv[0] << ": ";
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from player surface! Error: %s\n", SDL_GetError());
        return 1;
    }
    SDL_FreeSurface(playerSurface);
    
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
    }
    
    while(gameRunning){
        //Event Handling
        SDL_PollEvent(&event);
        if(event.type == SDL_QUIT){
            gameRunning = false;
        }
        if((event.type == SDL_CONTROLLERDEVICEADDED) && (!gamepadConnected)){
            int joysticks = SDL_NumJoysticks();
            for(int joystickIndex = 0; joystickIndex < joysticks; ++joystickIndex){
                if(!SDL_IsGameController(joystickIndex)){
                    continue;
                }
                cout << "Joystick at index: " << joystickIndex << " is a valid game controller\n";
                controller = SDL_GameControllerOpen(joystickIndex);
                gamepadConnected = true;
            }
        }
        if(event.type == SDL_CONTROLLERDEVICEREMOVED){
            SDL_GameControllerClose(controller);
            gamepadConnected = false;
            cout << "Hit right here\n";
        }
        
        //Updating surfaces and rendering
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, background, NULL, NULL);
        //Add the player
        SDL_RenderCopy(renderer, playerTexture, NULL, &player);
        
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    if(gamepadConnected){
        SDL_GameControllerClose(controller);
    }
    SDL_Quit();
    return 0;
}