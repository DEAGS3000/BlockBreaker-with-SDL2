#include "Paddle.h"


Paddle::Paddle(void)
{
	set_pos(SCREEN_WIDTH / 2, 680);
	geometry.w = 100;
	geometry.h = 10;
	size_maximized = false;
}


Paddle::~Paddle(void)
{
}


// ��䳤
void Paddle::size_up()
{
	if (!size_maximized)
	{
		int new_length = get_geometry().w * 2;
		geometry.x -= (new_length - geometry.w) / 2;
		set_width(new_length);
		size_maximized = true;
	}
}

// ���ò���--����
void Paddle::set_texture(SDL_Texture **textures)
{
	this->texture = textures[STICK];
}
