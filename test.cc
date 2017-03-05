#include <iostream>
#include "SDL.h"
#include <string>

using namespace std;

//Function definitions
SDL_Texture *loadTexture(string image, SDL_Renderer *renderer);

int main(int argc, char *argv[]){
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0){
        SDL_Log("Unable to initialize SDL: %s\n", SDL_GetError());
    }
    
    //Pointers for our window and renderer, and controller
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_GameController *controller;
    //Pointers to our textures
    SDL_Texture *background;
    SDL_Texture *playerTexture;
    //An event to be polled
    SDL_Event event;
    bool gameRunning = true;
    
    //Player rect
    SDL_Rect player;
    player.x = 0;
    player.y = 440;
    player.w = 40;
    player.h = 40;
    
    //Creating a window
    if(SDL_CreateWindowAndRenderer(640, 480, SDL_WINDOW_OPENGL, &window, &renderer)){
        cerr << argv[0] << ": ";
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create window and renderer! Error: %s\n", SDL_GetError());
        return 1;
    }
    //Loading the background image as a surface
    background = loadTexture(string("./images/wht-marble.bmp"), renderer);
    //Loading player surface, creating texture
    playerTexture = loadTexture(string("./images/Smiley.bmp"), renderer);
    
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
        //In case of quit
        if(event.type == SDL_QUIT){
            gameRunning = false;
        }
        //If a controller is connected during run
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
        //If controller is disconnected during run
        if(event.type == SDL_CONTROLLERDEVICEREMOVED){
            SDL_GameControllerClose(controller);
            gamepadConnected = false;
        }
        
        //Keyboad input (for testing)
        if(event.type == SDL_KEYDOWN){
            switch(event.key.keysym.sym){
                case SDLK_RIGHT:
                    player.x++;
                    break;
                case SDLK_LEFT:
                    player.x--;
                    break;
            }
        }
        
        //Check to keep player on screen
        if(player.x < 0){
            player.x = 0;
        }
        if(player.x > 600){
            player.x = 600;
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

/*
Function that takes an image path, creates a surface from the image, and then a texture from the surface
Params: the path to the image we're using as a c++ string, pointer to the renderer to be used
*/
SDL_Texture *loadTexture(string image, SDL_Renderer *renderer){
        SDL_Surface *surface = SDL_LoadBMP(image.data());
    if(!surface){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't load surface from bmp: %s Error: %s\n", image.data(), SDL_GetError());
        return NULL;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if(!texture){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface for image: %s Error: %s\n", image.data(), SDL_GetError());
        return NULL;
    }
    SDL_FreeSurface(surface);
    return texture;
}