#pragma once
#include "ball.h"
class GravityBall :
	public Ball
{
public:
	GravityBall(void);
	~GravityBall(void);
	void set_texture(SDL_Texture **textures);
	Ball* clone();
};

