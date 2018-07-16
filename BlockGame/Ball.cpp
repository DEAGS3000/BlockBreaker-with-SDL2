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
	cout << "Ball::hit������" << endl;
	switch(direction)
	{
	case TOP:
			speed.y *= -1;
			cout << "С����ש�鶥��" << endl;
			inverted = true;
		break;
	case BOTTOM:
			speed.y *= -1;
			cout << "С����ש��ײ�" << endl;
			inverted = true;
		break;
	case LEFT:
		if(!(speed.x<0.0f))
		{
			speed.x *= -1;
			cout << "С����ש����" << endl;
			inverted = true;
		}
		break;
	case RIGHT:
		if(!(speed.x>0.0f))
		{
			speed.x *= -1;
			cout << "С����ש���Ҳ�" << endl;
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


// ����С��
void Ball::launch()
{
	launched = true;
	// ֻ����ԭ��û���ٶȵ�����²ų�ʼ���ٶ�
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

// Ϊ�˱���С���ƶ����쵼�·�Խ���������������ÿ����һ��ֻ�ƶ�һ�룬Ȼ������ײһ�Σ����ƶ�һ��
void Ball::update(float delta_time)
{
	last_pos.x = geometry.x;
	last_pos.y = geometry.y;
	geometry.x += speed.x * delta_time / 2.0f;
	geometry.y += speed.y * delta_time / 2.0f;
}
