#pragma once
#include "block.h"
class SteelBlock :
	public Block
{
public:
	SteelBlock(void);
	SteelBlock(int x, int y);
	~SteelBlock(void);

void hit_by(Ball* ball, list<Item*> *l);
	void set_texture(SDL_Texture **textures);
	Ball* clone();
};

