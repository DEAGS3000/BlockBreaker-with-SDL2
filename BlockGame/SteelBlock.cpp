#include "SteelBlock.h"


SteelBlock::SteelBlock(void)
{
	health = 100;
	score = 50;
}

SteelBlock::SteelBlock(int x, int y)
{
	SteelBlock();
	geometry.x = x;
	geometry.y = y;
	health = 100;
	score = 50;
}


SteelBlock::~SteelBlock(void)
{
}

void SteelBlock::hit_by(Ball* ball, list<Item*> *l)
{
	health -= ball->damage;
	texture = textures[STEEL_BLOCK];
}

void SteelBlock::set_texture(SDL_Texture **textures)
{
	this->texture = textures[STEEL_BLOCK];
	this->textures = textures;
}
