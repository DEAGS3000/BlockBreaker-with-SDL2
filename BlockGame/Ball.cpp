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


// ����С��
void Ball::launch()
{
	launched = true;
	// ֻ����ԭ��û���ٶȵ�����²ų�ʼ���ٶ�
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

// Ϊ�˱���С���ƶ����쵼�·�Խ���������������ÿ����һ��ֻ�ƶ�һ�룬Ȼ������ײһ�Σ����ƶ�һ��
void Ball::update(float delta_time)
{

	geometry.x += speed.x * delta_time / 2;
	geometry.y += speed.y * delta_time / 2;
}
