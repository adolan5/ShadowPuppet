#include <iostream>
#include "SDL.h"
#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL_opengl.h>
#include <string>

using namespace std;

//Function declarations
SDL_Texture *loadTexture(string image, SDL_Renderer *renderer);
int initialize();
void quitGame();
bool initializeGL();

//Private variables
//Pointers for our window and renderer, and controller
static SDL_Window *window;
static SDL_GameController *controller;
//OpenGL context and other variables
static SDL_GLContext gameContext;
static GLuint programID = 0;
static GLint vertex = -1;
static GLuint vbo = 0;
static GLuint ibo = 0;
//Controller deadzone
static const int deadzone = 8000;
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
        cerr << argv[0] << ": Unable to initialize, exiting\n";
        quitGame();
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
    }
    quitGame();
    return 0;
}

//Function that initializes SDL and other libraries
int initialize(){
    //Starting SDL
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0){
        SDL_Log("Unable to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }
    
    //Creating a window
    window = SDL_CreateWindow("ShadowPuppet", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    if(window == NULL){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create window! Error: %s\n", SDL_GetError());
        return 1;
    }
    
    //TODO: Initializing OpenGL
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    //Create OpenGL context
    gameContext = SDL_GL_CreateContext(window);
    if(gameContext == NULL){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create OpenGL context. Error: %s", SDL_GetError());
        return 1;
    }
    glewExperimental = GL_TRUE;
    
    //Use vSync
    if(SDL_GL_SetSwapInterval(1) < 0){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to set vSync. Error: %s", SDL_GetError());
        return 1;
    }
    //Initialize OpenGL
    if(!initializeGL()){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to initialize OpenGL! Error: %s", SDL_GetError());
        return 1;
    }
    
    //Setting player initial position
    player.x = 0;
    player.y = 440;
    player.w = 40;
    player.h = 40;
    
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

bool initializeGL(){
    GLenum error = GL_NO_ERROR;
    glMatrixMode(GL_PROJECTION);
    //Replaces current matrix with identity matrix
    glLoadIdentity();
    error = glGetError();
    if(error != GL_NO_ERROR){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize OpenGL. Error: %s\n", gluErrorString(error));
        return false;
    }
    return true;
}

//Function to shut down all libraries and exit
void quitGame(){
    SDL_DestroyWindow(window);
    if(gamepadConnected){
        SDL_GameControllerClose(controller);
    }
    SDL_Quit();
}

/*
Function that takes an image path, creates a surface from the image, and then a texture from the surface
Params: the path to the image we're using as a c++ string, pointer to the renderer to be used
TODO: Remove if no longer used
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
