#pragma once
#include "block.h"
class ItemBlock :
	public Block
{
public:
	ItemBlock(void);
	~ItemBlock(void);
	ItemBlock(int x, int y);
	void set_texture(SDL_Texture **textures);
	// ×©¿é±»»÷ÖÐ
	void hit_by(Ball *ball, list<Item*> *l);
};

