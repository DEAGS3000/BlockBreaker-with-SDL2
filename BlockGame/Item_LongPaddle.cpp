#include "Item_LongPaddle.h"


Item_LongPaddle::Item_LongPaddle(void)
{
}


Item_LongPaddle::~Item_LongPaddle(void)
{
}


// 发动效果
/*void Item_LongPaddle::take_effect(Game *game, Ball *ball)
{
	game->paddle.size_up();
}*/


void Item_LongPaddle::set_texture(SDL_Texture **textures)
{
	this->texture = textures[ITEM_LONG_PADDLE];
}
