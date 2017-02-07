#include "NormalBall.h"


NormalBall::NormalBall(void)
{
	damage = 1;
}


NormalBall::~NormalBall(void)
{
}


// ���ò���--����
void NormalBall::set_texture(SDL_Texture **textures)
{
	this->texture = textures[NORMAL_BALL];
	this->textures = textures;
}

// ��¡
Ball* NormalBall::clone()
{
	Ball *temp = new NormalBall;
	temp->set_geometry(geometry);
	temp->set_texture(textures);
	temp->speed = speed;
	// ��ˮƽ�ٶ��������һ��Ķ�
	//int delta_x[10] = {-5, -4, -3, -2, -1, 1, 2, 3, 4, 5};
	//temp->speed.x += delta_x[rand()%19];
	temp->launch();
	return temp;
}
