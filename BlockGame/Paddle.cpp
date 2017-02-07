#include "Paddle.h"


Paddle::Paddle(void)
{
	set_pos(SCREEN_WIDTH/2, 680);
	geometry.w = 100;
	geometry.h = 10;
}


Paddle::~Paddle(void)
{
}


// 板变长
void Paddle::size_up()
{
	int new_length = get_geometry().w*2;
	geometry.x -= (new_length-geometry.w)/2;
	set_width(new_length);
}

// 设置材质--重载
void Paddle::set_texture(SDL_Texture **textures)
{
	this->texture = textures[STICK];
}
