#ifndef PLAYER_HEADER
#define PLAYER_HEADER

class Player{

	public: 
	int x, y, xvel, yvel, width, height;
	bool falling;
	Player(int, int);
	void jump();
	void moveUp();
	void ifFall();
	void moveLeft();
	void moveRight();
	void borderCollision();
};

#endif
