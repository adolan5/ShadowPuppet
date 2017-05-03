//Class implementation for controller functionality in ShadowPuppet
//Author: Andy D.
#include "Controller.h"

using namespace std;

//Ctor takes a deadzone
ShadowController::ShadowController(const int dz): deadzone(dz){		}

//Open a gamepad
bool ShadowController::openGamepad(){
	int joysticks = SDL_NumJoysticks();
    //Loop through the joysticks, if it's a valid controller, open it
    for(int joystickIndex = 0; joystickIndex < joysticks; ++joystickIndex){
        if(!SDL_IsGameController(joystickIndex)){
            continue;
        }
        controller = SDL_GameControllerOpen(joystickIndex);
		return true;
    }
	return false;
}

//Close a gamepad
void ShadowController::closeGamepad(){
	SDL_GameControllerClose(controller);
}

//Get the gamepad pointer
SDL_GameController* ShadowController::getController() const{
	return controller;
}

//Get the deadzone
int ShadowController::getDeadzone() const{
	return deadzone;
}
