#include "SuperBall.h"


SuperBall::SuperBall(void)
{
	damage = 100;
}


SuperBall::~SuperBall(void)
{
}

void SuperBall::set_texture(SDL_Texture **textures)
{
	this->texture = textures[SUPER_BALL];
	this->textures = textures;
}

void SuperBall::hit(int direction)
{
}

// 克隆
Ball* SuperBall::clone()
{
	Ball *temp = new SuperBall;
	temp->set_geometry(geometry);
	temp->set_texture(textures);
	temp->speed = speed;
	// 在水平速度上随机做一点改动
	//int delta_x[10] = {-5, -4, -3, -2, -1, 1, 2, 3, 4, 5};
	//temp->speed.x += delta_x[rand()%19];
	temp->launch();
	return temp;
}