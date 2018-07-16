#pragma once
#include "graphicitem.h"
#include "common_def.h"


class Paddle :
	public GraphicItem
{
public:
	Paddle(void);
	~Paddle(void);
	void size_up();
	// 限制滑板只能变长一次
	bool size_maximized;
	void set_texture(SDL_Texture **textures);
	//void update();

	//int speed;
	//void update();
};

