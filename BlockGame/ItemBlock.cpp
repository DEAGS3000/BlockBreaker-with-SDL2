#include "ItemBlock.h"


ItemBlock::ItemBlock(void)
{
}


ItemBlock::ItemBlock(int x, int y)
{
	geometry.x = x;
	geometry.y = y;
	score = 0;
	health = 1;
}


ItemBlock::~ItemBlock(void)
{
}


void ItemBlock::set_texture(SDL_Texture **textures)
{
	this->texture = textures[ITEM_BLOCK];
	this->textures = textures;
}
	// ×©¿é±»»÷ÖÐ
void ItemBlock::hit_by(Ball *ball, list<Item*> *l)
{
	health -= ball->damage;
	//l->push_back(new Item(ITEM_LONG_PADDLE, geometry.x, geometry.y, textures));
	//l->push_back(new Item(ITEM_IMPROVE_BALL, geometry.x, geometry.y, textures));
	//l->push_back(new Item(ITEM_TRIPLE_BALL, geometry.x, geometry.y, textures));
	//l->push_back(new Item(ITEM_ADD_LIFE, geometry.x, geometry.y, textures));
	l->push_back(new Item(rand()%(18 - 15 +1) + 15, geometry.x, geometry.y, textures));
}
