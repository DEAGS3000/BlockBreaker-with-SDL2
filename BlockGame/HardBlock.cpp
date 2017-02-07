#include "HardBlock.h"


HardBlock::HardBlock(void)
{
	/*geometry.w = 80;
	geometry.h = 30;*/
	// ÉúÃüÖµÎª2
	health = 2;
	score = 10;
}
HardBlock::HardBlock(int x, int y)
{
	HardBlock();
	geometry.x = x;
	geometry.y = y;
	health = 2;
	score = 10;
}


HardBlock::~HardBlock(void)
{
}

void HardBlock::set_texture(SDL_Texture **textures)
{
	this->texture = textures[HARD_BLOCK];
	this->textures = textures;
}

void HardBlock::hit_by(Ball* ball, list<Item*> *l)
{
	health -= ball->damage;
	texture = textures[HARD_BLOCK_CRACKED];
}
