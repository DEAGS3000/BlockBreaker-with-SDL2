#include "Ball.h"


Ball::Ball(void)
{
	//set_geometry(0, 0, 0, 0);
	launched = false;
	geometry.h = 15;
	geometry.w = 15;
	crashed = false;
}


Ball::~Ball(void)
{
}

void Ball::hit(int direction)
{
	switch(direction)
	{
	case TOP:
		if(!(speed.y<0.0f))
			speed.y *= -1;
		break;
	case BOTTOM:
		if(!(speed.y>0.0f))
			speed.y *= -1;
		break;
	case LEFT:
		if(!(speed.x<0.0f))
		    speed.x *= -1;
		break;
	case RIGHT:
		if(!(speed.x>0.0f))
			speed.x *= -1;
		break;
	}
}


// 发射小球
void Ball::launch()
{
	launched = true;
	// 只有在原来没有速度的情况下才初始化速度
	if(speed.x==0 && speed.y==0)
	{
		speed.x = 0;
		speed.y = 10;
	}
}


bool Ball::is_launched()
{
	return launched;
}

double Ball::radius()
{
	return geometry.w / 2;
}

// 为了避免小球移动过快导致飞越，重载这个函数，每调用一次只移动一半，然后检测碰撞一次，再移动一半
void Ball::update(float delta_time)
{

	geometry.x += speed.x * delta_time / 2;
	geometry.y += speed.y * delta_time / 2;
}
