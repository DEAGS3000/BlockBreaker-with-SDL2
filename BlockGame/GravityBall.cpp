#include "GravityBall.h"


GravityBall::GravityBall(void)
{
	damage = 2;
}


GravityBall::~GravityBall(void)
{
}


// 设置材质--重载
void GravityBall::set_texture(SDL_Texture **textures)
{
	this->texture = textures[GRAVITY_BALL];
	this->textures = textures;
}


// 克隆
Ball* GravityBall::clone()
{
	Ball *temp = new GravityBall;
	temp->set_geometry(geometry);
	temp->set_texture(textures);
	temp->speed = speed;
	// 在水平速度上随机做一点改动
	//int delta_x[10] = {-5, -4, -3, -2, -1, 1, 2, 3, 4, 5};
	//temp->speed.x += delta_x[rand()%19];
	temp->launch();
	return temp;
}