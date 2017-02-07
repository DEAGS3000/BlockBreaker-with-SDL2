#pragma once
#include "block.h"
class NormalBlock :
	public Block
{
public:
	NormalBlock(void);
	NormalBlock(int x, int y);
	~NormalBlock(void);

	void set_texture(SDL_Texture **textures);

};

