#include "Player.h"
#include <iostream>

using namespace std;
	
	
	Player::Player(int x, int y){
	this->x = x;
	this->y = y;
	xvel = 5;
	height = 40;
	width = 40;
	falling = false;
	}

	// all jumping/falling
	void Player::jump(){
		if(!falling){
		    yvel = -15;
		    falling = true;
		}
		borderCollision();
	}	
	//moves in the respective direction
	void Player::moveUp(){
		jump();
		y+= yvel;
		borderCollision();
	}
	void Player::ifFall(){
		if(falling){
			yvel += 1;
			y+= yvel;
			if(yvel > 28){
				yvel = 28;	//Cap for the yVel, just to be safe
			}
		}
		borderCollision();
	}
	void Player::moveLeft(){
		x-= xvel;
		borderCollision();
	}
	void Player::moveRight(){
		x+= xvel;
		borderCollision();
	}
	//border collision take into account that it looks at top left corner
	void Player::borderCollision(){
		//this case accounts for hitting the ground
		if(y + height > 720){
		   falling = false;
		   yvel = 0;
		   y = 720-height;
		}
		// if it the player tries to go past the roof
		if(y <= 0){
		   falling = true;
		   y = 0+height;
		}
		//the left side...
		if(x <= 0){
		   x = 0;
		}
		//finally the right
		if(x + width>= 1080){	
		   x = 1080-width;
		}
	
	}
	

/* int main(){
	Player p(0,10);
	//testing the starting coordinates
	cout << "should be 0: " << p.x << " should be 40: "<< p.y << " should be false:" <<  p.falling << "\n";
	p.moveUp();
	p.ifFall();	

	cout << "should be 0: " << p.x << " should be 440: "<< p.y << "\n";

} */
