#include "Ball.h"
#include <iostream>

using namespace std;


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
	bool inverted = false;
	cout << "Ball::hit被调用" << endl;
	switch(direction)
	{
	case TOP:
			speed.y *= -1;
			cout << "小球触碰砖块顶部" << endl;
			inverted = true;
		break;
	case BOTTOM:
			speed.y *= -1;
			cout << "小球触碰砖块底部" << endl;
			inverted = true;
		break;
	case LEFT:
		if(!(speed.x<0.0f))
		{
			speed.x *= -1;
			cout << "小球触碰砖块左部" << endl;
			inverted = true;
		}
		break;
	case RIGHT:
		if(!(speed.x>0.0f))
		{
			speed.x *= -1;
			cout << "小球触碰砖块右部" << endl;
			inverted = true;
		}
		break;
	}
	if (!inverted)
	{
		int a = 0;
		int b = 1;
		int c = a + b;
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
		speed.y = 8;
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
	last_pos.x = geometry.x;
	last_pos.y = geometry.y;
	geometry.x += speed.x * delta_time / 2.0f;
	geometry.y += speed.y * delta_time / 2.0f;
}
