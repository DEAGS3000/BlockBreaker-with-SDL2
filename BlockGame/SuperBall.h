#pragma once
#include "ball.h"
class SuperBall :
	public Ball
{
public:
	SuperBall(void);
	~SuperBall(void);
	void set_texture(SDL_Texture **textures);
	void hit(int direction);
	Ball* clone();
};

