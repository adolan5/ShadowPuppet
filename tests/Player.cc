#include "Player.h"
#include <iostream>

using namespace std;
	
	
	Player::Player(int x, int y){
	this->x = x;
	this->y = y;
	this->xvel = 5;
	this->width = 40;
	this->falling = false;
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
		if(y + height > 480){
		   falling = false;
		   yvel = 0;
		   y = 440;
		}
		// if it the player tries to go past the roof
		if(y <= 0){
		   falling = true;
		   y = 0;
		}
		//the left side...
		if(x <= 0){
		   x = 0;
		}
		//finally the right
		if(x + width>= 640){	
		   x = 600;
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
