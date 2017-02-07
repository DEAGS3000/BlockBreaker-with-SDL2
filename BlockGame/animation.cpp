#include "animation.h"


Animation::Animation(void)
{
}

Animation::Animation(SDL_Texture *texture, int x, int y)
{
	this->texture = texture;
	this->source_rect.x = 0;
	this->source_rect.y = 0;
	this->source_rect.w = 111;
	this->source_rect.h = 111;
	this->target_rect.x = x;
	this->target_rect.y = y;
	this->target_rect.w = 70;
	this->target_rect.h = 35;
	frame = 0;
}

void Animation::update(int frame)
{
	this->frame = frame;
	source_rect.x = 111 * (frame % 6);
	source_rect.y = 111 * (frame / 6);
}


void Animation::update2()
{
	this->frame++;
	source_rect.x = 111 * (frame % 6);
	source_rect.y = 111 * (frame / 6);
}


Animation::~Animation(void)
{
}
