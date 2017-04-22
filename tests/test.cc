#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include "shadowRender.h"

using namespace std;

//Function declarations
void openGamepad();
int initialize();
void quitGame();
void playGame();
void generatePlatforms(vector<pair<int, int> > coordPairs); //TODO: Integration with Kinect stuff!
int collision();

/*          Private variables       */
//Window dimensions
static const int WINDOW_HEIGHT = 480;
static const int WINDOW_WIDTH = 640;
//Pointers for our window and renderer, and controller
static SDL_Window *window;
static SDL_GameController *controller;
//Controller deadzone
static const int deadzone = 8000;
//An event to be polled
static SDL_Event event;
//Bool's for if the game is running and if a controller has been connected
static bool gameRunning = true;
static bool gamepadConnected = false;
//Player rect
static SDL_Rect player;
//X and Y velocities for our player
static const int xVel = 5;
static int yVel = 0;
//Bool for if a render is even needed (save a screen swap)
static bool needRender = true;
static bool jumping = false;
//Vector of platform rects
static vector<SDL_Rect> platforms(20);
//Bool for if platforms are present
static bool platformsPresent = false;
//Number of platforms
static int numPlatforms = 0;

//SHADOW RENDERER
static ShadowRenderer renderer(WINDOW_HEIGHT, WINDOW_WIDTH);

//A vector of pairs, for TESTING ONLY!
vector<pair<int,int> > testVec = {make_pair(400, 400), make_pair(285, 360), make_pair(100, 280)};

//Function that initializes SDL and other libraries
int initialize(){
    //Starting SDL
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0){
        SDL_Log("Unable to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }
    
    //Creating a window
    window = SDL_CreateWindow("ShadowPuppet", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    if(window == NULL){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create window! Error: %s\n", SDL_GetError());
        return 1;
    }
	
	if(!renderer.initializeGL(window)){
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create OpenGL context. Error: %s\n", SDL_GetError());
		return 1;
	}
	
    //Setting player initial position
    player.x = 0;
    player.y = 440;
    player.w = 40;
    player.h = 40;
    
    //Opening the gamepad if one is connected
    openGamepad();
    return 0;
}

//Function to shut down all libraries and exit
void quitGame(){
    SDL_DestroyWindow(window);
    if(gamepadConnected){
        SDL_GameControllerClose(controller);
    }
    SDL_Quit();
	//Note: ShadowRenderer's dtor takes care of destroying textures and shutting down SDL_Image
}

//Function to open a gamepad
void openGamepad(){
    int joysticks = SDL_NumJoysticks();
    //Loop through the joysticks, if it's a valid controller, open it
    for(int joystickIndex = 0; joystickIndex < joysticks; ++joystickIndex){
        if(!SDL_IsGameController(joystickIndex)){
            continue;
        }
        controller = SDL_GameControllerOpen(joystickIndex);
        gamepadConnected = true;
    }
}

//Main game function
void playGame(){
	//Main game loop
    while(gameRunning){
        //TODO: Keep/fix delay for smoothness?
        SDL_Delay(10); //Trying to use this, keeping controller handling out of event polling
        
        //Event Handling
        SDL_PollEvent(&event);
        //In case of quit
        if(event.type == SDL_QUIT){
            gameRunning = false;
        }
        //If a controller is connected during run
        if((event.type == SDL_CONTROLLERDEVICEADDED) && (!gamepadConnected)){
            openGamepad();
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
                    player.x += xVel;
                    needRender = true;
					if(collision() != -1){
						player.x -= xVel;
						needRender = false;
					}
                    break;
                case SDLK_LEFT:
                    player.x -= xVel;
                    needRender = true;
					if(collision() != -1){
						player.x += xVel;
						needRender = false;
					}
                    break;
                case SDLK_ESCAPE:
                    gameRunning = false;
                    break;
				case SDLK_SPACE:
					if(!jumping){
						jumping = true;
						yVel = 20;
					}
					break;
				case SDLK_x:
					generatePlatforms(testVec);
					needRender = true;
					break;
            }
        }
		
        //Controller input
        if(gamepadConnected){
            if(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX) > deadzone){
                player.x += xVel;
                needRender = true;
            }
            if(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX) < -deadzone){
                player.x -= xVel;
                needRender = true;
            }
            if(SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A) == 1){
				if(!jumping){
					jumping = true;
					yVel = 20;
				}
			}
			if(SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_B) == 1){
				gameRunning = false;
			}
			if(SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_X) == 1){
				generatePlatforms(testVec);
			}
        }
        //Update player's y position, only happens when jumping (or falling)
		player.y -= yVel;
		needRender = true;
		int collisionCheck = collision();
		if(collisionCheck != -1){
			if(player.y < platforms[collisionCheck].y){
				player.y = (platforms[collisionCheck].y - player.h);
				yVel = -1;
				jumping = false;
			}else{
				player.y = (platforms[collisionCheck].y + platforms[collisionCheck].h);
				yVel = -10;
			}
		}
        
        //Keeping the player on screen
        if(player.x < 0){
            player.x = 0;
        }
        if(player.x > 600){
            player.x = 600;
        }
        if(player.y > 440){
			player.y = 440;
			yVel = 0;
			jumping = false; //This line for if we've been falling
		}
		if(player.y < 0){
			player.y = 0;
			yVel = -10;
		}
		//Updating yVel
		yVel--;
        //Rendering to screen
        if(needRender){
            renderer.glRender(player, platformsPresent, numPlatforms, platforms);
            SDL_GL_SwapWindow(window);
            needRender = false;
        }
    }
}
//Function to generate platforms; takes in a vector of coords TODO: Kinect stuff goes here
void generatePlatforms(vector<pair<int, int> > coordPairs){
	//Need to clear out the current platforms before making the new ones
	platforms.clear();
	numPlatforms = 0;
	for(auto v : coordPairs){
		platforms[numPlatforms].x = v.first;
		platforms[numPlatforms].y = v.second;
		platforms[numPlatforms].w = 100;
		platforms[numPlatforms].h = 25;
		numPlatforms++;
	}
	platformsPresent = true;
}
//Function to check if the player has collided with any platforms, returns the index of the platform collided with
int collision(){
	//Can't have collisions if there are no platforms present
	if(!platformsPresent){
		return -1;
	}
	for(int i = 0; i < numPlatforms; i++){
		if(((player.x + player.w) > platforms[i].x) && (player.x < (platforms[i].x + platforms[i].w))){
			if(((player.y + player.h) > platforms[i].y) && (player.y < (platforms[i].y + platforms[i].h))){
				return i;
			}
		}
	}
	return -1;
}
//Main
int main(int argc, char *argv[]){
    if(initialize() == 1){
        cerr << argv[0] << ": Unable to initialize, exiting\n";
        quitGame();
        return 1;
    }
    playGame();
    quitGame();
    return 0;
}