#include "shadowRender.h"
#include "Player.h"
#include "shadowController.h"
#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_opengl.h>
#include <vector>

using namespace std;

//Function declarations
int initialize();
void quitGame();
void playGame();
void generatePlatforms(vector<pair<int, int> > coordPairs); //TODO: Integration with Kinect stuff!
int collision();

/*          Private variables       */
//Pointers for our window and renderer, and controller
static SDL_Window *window;
//An event to be polled
static SDL_Event event;
//Keyboard state
static const Uint8 *state;
//Bool's for if the game is running and if a controller has been connected
static bool gameRunning = true;
static bool gamepadConnected = false;
//Bool for if a render is even needed (save a screen swap)
static bool needRender = true;
//Vector of platform rects
static vector<SDL_Rect> platforms;
//Bool for if platforms are present
static bool platformsPresent = false;

//Player instance (Courtesy of Bhua)
static Player player(0, 440);
//SHADOW RENDERER (Yay organization!)
static ShadowRenderer renderer(480, 640);

//SHADOW CONTROLLER
static ShadowController ctrl(8000);

//A vector of pairs, for TESTING ONLY!
vector<pair<int,int> > testVec = {make_pair(400, 400), make_pair(285, 360), make_pair(100, 280), make_pair(250, 100)};

//Function that initializes SDL and other libraries
int initialize(){
    //Starting SDL
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0){
        SDL_Log("Unable to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }
    
    //Creating a window
    window = SDL_CreateWindow("ShadowPuppet", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, renderer.getWidth(), renderer.getHeight(), SDL_WINDOW_OPENGL);
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
    player.width = 40;
    player.height = 40;
    
    //Opening the gamepad if one is connected
    gamepadConnected = ctrl.openGamepad();
	
	//Getting keyboard state
	state = SDL_GetKeyboardState(NULL);
	
    return 0;
	
}

//Function to shut down all libraries and exit
void quitGame(){
    SDL_DestroyWindow(window);
    if(gamepadConnected){
        ctrl.closeGamepad();
    }
    SDL_Quit();
	//Note: ShadowRenderer's dtor takes care of destroying textures and shutting down SDL_Image
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
            gamepadConnected = ctrl.openGamepad();
        }
        //If controller is disconnected during run
        if(event.type == SDL_CONTROLLERDEVICEREMOVED){
            ctrl.closeGamepad();
            gamepadConnected = false;
        }
        
        //Keyboad input (for testing)
		if(state[SDL_SCANCODE_RIGHT]){
			player.moveRight();
			needRender = true;
			if(collision() != -1){
				player.x -= player.xvel;
				needRender = false;
			}
		}else if(state[SDL_SCANCODE_LEFT]){
			player.moveLeft();
			needRender = true;
			if(collision() != -1){
				player.x += player.xvel;
				needRender = false;
			}
		}
		if(state[SDL_SCANCODE_SPACE]){
			player.jump();
		}
		if(state[SDL_SCANCODE_ESCAPE]){
			gameRunning = false;
		}
		if(state[SDL_SCANCODE_X]){
			generatePlatforms(testVec);
			needRender = true;
		}
		
        //Controller input
        if(gamepadConnected){
            if(SDL_GameControllerGetAxis(ctrl.getController(), SDL_CONTROLLER_AXIS_LEFTX) > ctrl.getDeadzone()){ //Alter to if(shadowController.movingLeft())? Or abstract entire controller checking section into shadowController.cc?
                player.moveRight();
                needRender = true;
				if(collision() != -1){
					player.x -= player.xvel;
					needRender = false;
				}
            }
            if(SDL_GameControllerGetAxis(ctrl.getController(), SDL_CONTROLLER_AXIS_LEFTX) < -(ctrl.getDeadzone())){
                player.moveLeft();
                needRender = true;
				if(collision() != -1){
					player.x += player.xvel;
					needRender = false;
				}
            }
            if(SDL_GameControllerGetButton(ctrl.getController(), SDL_CONTROLLER_BUTTON_A) == 1){
				player.jump();
			}
			if(SDL_GameControllerGetButton(ctrl.getController(), SDL_CONTROLLER_BUTTON_B) == 1){
				gameRunning = false;
			}
			if(SDL_GameControllerGetButton(ctrl.getController(), SDL_CONTROLLER_BUTTON_X) == 1){
				generatePlatforms(testVec);
			}
        }
        //Update player's y position, only happens when jumping (or falling)
		player.ifFall();
		needRender = true;
		int collisionCheck = collision();
		if(collisionCheck != -1){
			if(player.y < platforms[collisionCheck].y){
				player.y = (platforms[collisionCheck].y - player.height);
				player.yvel = 1;
				player.falling = false;
			}else{
				player.y = (platforms[collisionCheck].y + platforms[collisionCheck].h);
				player.yvel = 10;
			}
		}
		else if(player.y < 440){
			player.falling = true;
		}
		
        //Rendering to screen
        if(needRender){
            renderer.glRender(player, platformsPresent, platforms);
            SDL_GL_SwapWindow(window);
            needRender = false;
        }
    }
}
//Function to generate platforms; takes in a vector of coords TODO: Kinect stuff goes here
void generatePlatforms(vector<pair<int, int> > coordPairs){
	//Need to clear out the current platforms before making the new ones
	platforms.clear();
	for(auto v : coordPairs){
		//Make a new rect and push it onto the platform vector
		SDL_Rect temp;
		temp.x = v.first;
		temp.y = v.second;
		temp.w = 100;
		temp.h = 25;
		platforms.push_back(temp);
	}
	platformsPresent = true;
}
//Function to check if the player has collided with any platforms, returns the index of the platform collided with
int collision(){
	//Can't have collisions if there are no platforms present
	if(!platformsPresent){
		return -1;
	}
	for(size_t i = 0; i < platforms.size(); i++){
		if(((player.x + player.width) > platforms[i].x) && (player.x < (platforms[i].x + platforms[i].w))){
			if(((player.y + player.height) > platforms[i].y) && (player.y < (platforms[i].y + platforms[i].h))){
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