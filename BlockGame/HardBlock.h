#pragma once
#include "block.h"
class HardBlock :
	public Block
{
public:
	HardBlock(void);
	HardBlock(int x, int y);
	~HardBlock(void);

	void set_texture(SDL_Texture **textures);
	void hit_by(Ball* ball, list<Item*> *l);
	Ball* clone();
};

