#include <iostream>
#include "SDL.h"
#include <string>

using namespace std;

//Function declarations
SDL_Texture *loadTexture(string image, SDL_Renderer *renderer);
int initialize();
void quitGame();

//Private variables
//Pointers for our window and renderer, and controller
static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_GameController *controller;
//Controller deadzone
static const int deadzone = 8000;
//Pointers to our textures
static SDL_Texture *background;
static SDL_Texture *playerTexture;
//An event to be polled
static SDL_Event event;
//Bool's for if the game is running and if a controller has been connected
static bool gameRunning = true;
static bool gamepadConnected = false;
//Player rect
static SDL_Rect player;
//X velocity for our player
static const int xVel = 1;

int main(int argc, char *argv[]){
    if(initialize() == 1){
        cerr << argv[0] << ": Unable to initialize, exiting";
        return 1;
    }
    //Main game loop
    while(gameRunning){
        //TODO: Keep/fix delay for smoothness?
        /*
        SDL_Delay(30); <--Commented out because of its unusual response time while a controller is connected
        I believe this bug comes from the OS not allowing such small slices of time to be handed out
        */
        
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
            cout << argv[0] << ": Detected that gamepad was disconnected\n";
        }
        
        //Keyboad input (for testing)
        if(event.type == SDL_KEYDOWN){
            switch(event.key.keysym.sym){
                case SDLK_RIGHT:
                    player.x += xVel;
                    break;
                case SDLK_LEFT:
                    player.x -= xVel;
                    break;
                case SDLK_ESCAPE:
                    gameRunning = false;
                    break;
            }
        }
        //Controller input
        if(gamepadConnected){
            if(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX) > deadzone){
                player.x += xVel;
            }
            if(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX) < -deadzone){
                player.x -= xVel;
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
    quitGame();
    return 0;
}

//Function that initializes SDL and other libraries
int initialize(){
    //Starting SDL
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0){
        SDL_Log("Unable to initialize SDL: %s\n", SDL_GetError());
    }
    //Setting player initial position
    player.x = 0;
    player.y = 440;
    player.w = 40;
    player.h = 40;
    //Creating a window
    if(SDL_CreateWindowAndRenderer(640, 480, SDL_WINDOW_OPENGL, &window, &renderer)){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create window and renderer! Error: %s\n", SDL_GetError());
        quitGame();
        return 1;
    }
    //Loading the background image as a surface
    background = loadTexture(string("../images/wht-marble24Bit.bmp"), renderer);
    if(background == NULL){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create background texture from passed string! Error: %s\n", SDL_GetError());
        quitGame();
        return 1;
    }
    //Loading player surface, creating texture
    playerTexture = loadTexture(string("../images/Smiley24Bit.bmp"), renderer);
    if(playerTexture == NULL){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create player texture from passed string! Error: %s\n", SDL_GetError());
        quitGame();
        return 1;
    }
    
    //Opening the gamepad if one is connected
    int joysticks = SDL_NumJoysticks();
    for(int joystickIndex = 0; joystickIndex < joysticks; ++joystickIndex){
        if(!SDL_IsGameController(joystickIndex)){
            continue;
        }
        cout << "Joystick at index: " << joystickIndex << " is a valid game controller\n";
        controller = SDL_GameControllerOpen(joystickIndex);
        gamepadConnected = true;
    }
    return 0;
}

//Function to shut down all libraries and exit
void quitGame(){
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    if(gamepadConnected){
        SDL_GameControllerClose(controller);
    }
    SDL_Quit();
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