#pragma once
#include "ball.h"
class NormalBall :
	public Ball
{
public:
	NormalBall(void);
	~NormalBall(void);

	void set_texture(SDL_Texture **textures);
	Ball* clone();
};

