/*Game driver for ShadowPuppet!*/
#include "Game.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]){
	//Instance of ShadowPuppet
	ShadowPuppet game;
	
	//Initialize
	if(game.initialize() == 1){
		cerr << argv[0] << ": Unable to initialize, exiting!\n";
		game.quitGame();
		return 1;
	}
	
	//Play, and then quit!
	game.playGame();
	game.quitGame();
	return 0;
}
