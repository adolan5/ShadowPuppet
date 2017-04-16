#include <iostream>
#include <string>
#include "SDL.h"
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>

using namespace std;

//Function declarations
void openGamepad();
bool loadTexture(string image, GLuint &textureID);
int initialize();
void quitGame();
bool initializeGL();
void glRender();
void playGame();
void generatePlatforms(vector<pair<int, int> > coordPairs); //TODO: Integration with Kinect stuff!
bool collision();

/*          Private variables       */
//Window dimensions
static const int WINDOW_HEIGHT = 480;
static const int WINDOW_WIDTH = 640;
//Pointers for our window and renderer, and controller
static SDL_Window *window;
static SDL_GameController *controller;
//OpenGL context and other variables
static SDL_GLContext gameContext;
static GLuint backgroundTexture = 0;
static GLuint playerTexture = 0;
static GLuint platformTexture = 0;
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
//Vector of platform rects(?)
static SDL_Rect platform;
static vector<SDL_Rect> platforms(20);
//Bool for if platforms are present
static bool platformsPresent = false;
//Number of platforms
static int numPlatforms = 0;

//A vector of pairs, for TESTING ONLY!
vector<pair<int,int> > testVec = {make_pair(400, 400), make_pair(285, 360)};

//Function that initializes SDL and other libraries
int initialize(){
    //Starting SDL
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0){
        SDL_Log("Unable to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }
    
    //Initializing SDL_image
    int image = IMG_Init(IMG_INIT_PNG);
    if((image & IMG_INIT_PNG) != IMG_INIT_PNG){
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL_image! Error: %s\n", IMG_GetError());
		return 1;
    }
    
    //Setting some SDL_GL attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    
    //Creating a window
    window = SDL_CreateWindow("ShadowPuppet", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    if(window == NULL){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create window! Error: %s\n", SDL_GetError());
        return 1;
    }
    
    //Create OpenGL context
    gameContext = SDL_GL_CreateContext(window);
    if(gameContext == NULL){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create OpenGL context. Error: %s\n", SDL_GetError());
        return 1;
    }
    
    //Initialize OpenGL
    if(!initializeGL()){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to initialize OpenGL! Error: %s\n", SDL_GetError());
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

//Function to start up OpenGL via OpenGL code
bool initializeGL(){
    GLenum error = GL_NO_ERROR;
    //Initialize projection matrix
    glMatrixMode(GL_PROJECTION);
    //Replaces current matrix with identity matrix
    glLoadIdentity();
    error = glGetError();
    if(error != GL_NO_ERROR){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize OpenGL.\n");
        return false;
    }
    //Initialize Modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    error = glGetError();
    if(error != GL_NO_ERROR){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize OpenGL.\n");
        return false;
    }
    //Initialize clear color
    glClearColor(0.f, 0.f, 0.f, 1.f);
    error = glGetError();
    if(error != GL_NO_ERROR){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize OpenGL.\n");
        return false;
    }
    //Setting up the orthographic coordinate system
    glOrtho(0.0, WINDOW_WIDTH, WINDOW_HEIGHT, 0.0, 1.0, -1.0);
	
    //These flags must be enabled for transparency of PNGs to work
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
    //Loading the textures we're using
    if(!loadTexture(string("../images/marblePng.png"), backgroundTexture)){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to load texture from string! Error: %s\n", SDL_GetError());
        return false;
    }
    if(!loadTexture(string("../images/cam.png"), playerTexture)){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to load texture from string! Error: %s\n", SDL_GetError());
        return false;
    }
    if(!loadTexture(string("../images/Platform.png"), platformTexture)){
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to load platform texture from string! Error: %s\n", SDL_GetError());
		return false;
	}
    return true;
}

//Function to shut down all libraries and exit
void quitGame(){
    //Delete textures now that we're done with them (and avoiding a very bad memory leak!)
    glDeleteTextures(1, &backgroundTexture);
    glDeleteTextures(1, &playerTexture);
    SDL_GL_DeleteContext(gameContext);
    SDL_DestroyWindow(window);
    if(gamepadConnected){
        SDL_GameControllerClose(controller);
    }
    IMG_Quit();
    SDL_Quit();
}

/*
Function that takes an image path, creates a surface from the image, and then a texture from the surface via OpenGL
Params: the path to the image we're using as a c++ string
*/
bool loadTexture(string image, GLuint &textureID){
    //Loading SDL surface from png via SDL_image
    SDL_Surface *surface = IMG_Load(image.data());
    if(!surface){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't load surface from image: %s Error: %s\n", image.data(), SDL_GetError());
        return false;
    }
    glGenTextures(1, &textureID);
    //Binding to the texture
    glBindTexture(GL_TEXTURE_2D, textureID);
    //Setting up the texture's settings
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //Actually creating the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    //Freeing the SDL surface, since we don't need it anymore
    SDL_FreeSurface(surface);
    return true;
}

/*
Funtion that does the OpenGL side of rendering textures that are loaded from images
*/
void glRender(){
    //Clear the context first
    glClear(GL_COLOR_BUFFER_BIT);
    //Need this to be enabled for this portion
    glEnable(GL_TEXTURE_2D);
    //Render background texture onto target
    glBindTexture(GL_TEXTURE_2D, backgroundTexture);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(0, 0);                           //Top left
        glTexCoord2f(1, 0); glVertex2f(WINDOW_WIDTH, 0);                //Top right
        glTexCoord2f(1, 1); glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);    //Bottom right
        glTexCoord2f(0, 1); glVertex2f(0, WINDOW_HEIGHT);               //Bottom left
    glEnd();
    //Render player
    glBindTexture(GL_TEXTURE_2D, playerTexture);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex2f(player.x,player.y);
        glTexCoord2f(1,0); glVertex2f((player.x + player.w),player.y);
        glTexCoord2f(1,1); glVertex2f((player.x + player.w),(player.y + player.h));
        glTexCoord2f(0,1); glVertex2f(player.x,(player.y + player.h));
    glEnd();
	//Render platform(s) if they're present
	if(platformsPresent){
		for(int i = 0; i < numPlatforms; i++){
			glBindTexture(GL_TEXTURE_2D, platformTexture);
			glBegin(GL_QUADS);
				glTexCoord2f(0,0); glVertex2f(platforms[i].x,platforms[i].y);
				glTexCoord2f(1,0); glVertex2f((platforms[i].x + platforms[i].w),platforms[i].y);
				glTexCoord2f(1,1); glVertex2f((platforms[i].x + platforms[i].w),(platforms[i].y + platforms[i].h));
				glTexCoord2f(0,1); glVertex2f(platforms[i].x,(platforms[i].y + platforms[i].h));
			glEnd();
		}
	}
    //Done with rendering, disable this flags
    glDisable(GL_TEXTURE_2D);
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
        //SDL_Delay(30); //<--Commented out because of its unusual response time while a controller is connected I believe this bug comes from mixing event polling and constant condition checking
        
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
					if(collision()){
						player.x -= xVel;
						needRender = false;
					}
                    break;
                case SDLK_LEFT:
                    player.x -= xVel;
                    needRender = true;
					if(collision()){
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
        //Controller buttons
        if(event.type == SDL_CONTROLLERBUTTONDOWN){
			switch(event.cbutton.button){
				case SDL_CONTROLLER_BUTTON_B:
					gameRunning = false;
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
        }
        //Update player's y position, only happens when jumping (or falling)
		player.y -= yVel;
		needRender = true;
		if(collision()){
			if(player.y < platform.y){
				player.y = (platform.y - player.h);
				yVel = -1;
				jumping = false;
			}else{
				player.y = (platform.y + platform.h);
				yVel = -10;
			}
		}
        
        //TODO: Replace these if conditions with collision detection
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
            glRender();
            SDL_GL_SwapWindow(window);
            needRender = false;
        }
    }
}
//Function to generate platforms (TODO: Maybe take in a vector of pairs, x and y coords?)
void generatePlatforms(vector<pair<int, int> > coordPairs){
	for(auto v : coordPairs){
		platforms[numPlatforms].x = v.first;
		platforms[numPlatforms].y = v.second;
		platforms[numPlatforms].w = 100;
		platforms[numPlatforms].h = 25;
		numPlatforms++;
	}
	//Platform widths and heights will always be the same
	platform.w = 100;
	platform.h = 25;
	platform.x = 125;
	platform.y = 400;
	platformsPresent = true;
}
//Function to check if the player has collided with any platforms
bool collision(){
	//Can't have collisions if there are no platforms present
	if(!platformsPresent){
		return false;
	}
	if(((player.x + player.w) > platform.x) && (player.x < (platform.x + platform.w))){
		if(((player.y + player.h) > platform.y) && (player.y < (platform.y + platform.h))){
			return true;
		}
	}
	return false;
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