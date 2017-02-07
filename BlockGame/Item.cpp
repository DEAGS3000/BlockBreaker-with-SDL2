#include "Item.h"


Item::Item(void)
{
	speed.y = 12;
	speed.x = 0;
	geometry.w= 30;
	geometry.h = 15;
	gotten = false;
}

Item::Item(int type, int x, int y, SDL_Texture **textures)
{
	gotten = false;
	this->type = type;
	speed.y = 6;
	speed.x = 0;
	geometry.x = x;
	geometry.y = y;
	geometry.w= 30;
	geometry.h = 15;
	set_texture(textures);
}

void Item::set_texture(SDL_Texture **textures)
{
	this->texture = textures[type];
}


Item::~Item(void)
{
}
