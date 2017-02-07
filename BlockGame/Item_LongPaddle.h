#pragma once
#include "item.h"
class Item_LongPaddle :
	public Item
{
public:
	Item_LongPaddle(void);
	~Item_LongPaddle(void);
	//void take_effect(Game *game, Ball *ball);
	void set_texture(SDL_Texture **textures);
};

