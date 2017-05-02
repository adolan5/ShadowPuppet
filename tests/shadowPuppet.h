/*Main class for ShadowPuppet!
  Author: Andy Dolan
*/
#ifndef GAME_INCLUDED
#define GAME_INCLUDED

#include "shadowRender.h"
#include "Player.h"
#include "shadowController.h"
#include <string>
#include <SDL.h>
#include <SDL_opengl.h>
#include <vector>

class ShadowPuppet{
	public:
		//Constructor initializes all the variables with their proper values
		ShadowPuppet();
		
		//Function to initialize SDL and other libraries
		int initialize();
		
		//Main game function
		void playGame();
		
		//Function to shut down all libraries and exit
		void quitGame();
		
		//Function to generate platforms; takes in a vector of coords. TODO: Kinect stuff goes here
		void generatePlatforms(std::vector<std::pair<int, int> > coordPairs);
		
		//Function to check if the player has collided with any platforms, returns the index of the platform collided with
		int collision();
		
	private:
		/*          Private variables       */
		//Pointers for our window and renderer, and controller
		SDL_Window *window;
		
		//An event to be polled
		SDL_Event event;
		
		//Keyboard state
		const Uint8 *state;
		
		//Player instance (Courtesy of Bhua)
		Player player;
		
		//Bool's for if the game is running and if a controller has been connected
		bool gameRunning;
		bool gamepadConnected;
		
		//Bool for if a render is even needed (save a screen swap)
		bool needRender;
		
		//Bool for if platforms are present
		bool platformsPresent;
		
		//Vector of platform rects
		std::vector<SDL_Rect> platforms;
		

		
		//SHADOW RENDERER (Yay organization!)
		ShadowRenderer renderer;

		//SHADOW CONTROLLER
		ShadowController ctrl;

		//A vector of pairs, for TESTING ONLY!
		std::vector<std::pair<int,int> > testVec = {std::make_pair(400, 400), std::make_pair(285, 360), std::make_pair(100, 280), std::make_pair(250, 100), std::make_pair(0,480)};
};


#endif