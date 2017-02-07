#pragma once
#include <SDL.h>
#include "common_def.h"

class Animation
{
public:
	SDL_Texture *texture;
	SDL_Rect source_rect;
	SDL_Rect target_rect;

	// 记录当前的帧数
	int frame;

	Animation(void);
	Animation(SDL_Texture *texture, int x, int y);
	void update(int frame);
	void update2();
	~Animation(void);
};

