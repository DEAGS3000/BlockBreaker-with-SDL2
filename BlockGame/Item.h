#pragma once
#include "graphicitem.h"

// Ô¤ÏÈÉùÃ÷


class Item :
	public GraphicItem
{
public:
	Item(void);
	Item(int type, int x, int y, SDL_Texture **textures);
	~Item(void);
	bool gotten;
	int type;
	void set_texture(SDL_Texture **textures);

	//virtual void take_effect(Game *game, Ball *ball) = 0;
};

