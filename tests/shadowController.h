//Class to handle controller functionality in ShadowPuppet
#ifndef CONTROLLER_INCLUDED
#define CONTROLLER_INCLUDED

#include <SDL.h>

class ShadowController{
	public:
		//Constructor takes a deadzone for the controller
		ShadowController(const int dz);
		
		//Open a gamepad, returns false if one is not found, returns true if one is successfully opened
		bool openGamepad();
		
		//Close a gamepad
		void closeGamepad();
		
		//Get controller pointer
		SDL_GameController* getController() const;
		
		//Get deadzone
		int getDeadzone() const;
		
	private:
		SDL_GameController *controller;
		const int deadzone;
	
};

#endif