#include "Block.h"
#include <cmath>


Block::Block(void)
{
	geometry.w = 50;
	geometry.h = 20;
}


Block::~Block(void)
{
}

void Block::hit_by(Ball* ball, list<Item*> *l)
{
	health -= ball->damage;
}


// 获取中点与顶点连线与X轴正半轴的夹角
double Block::right_bottom_angle()
{
	double x1 = geometry.x + geometry.w / 2;
	double y1 = geometry.y + geometry.h / 2;
	double x2 = geometry.x + geometry.w;
	double y2 = geometry.y + geometry.h;
	double angle = atan((y2-y1)/(x2-x1))*180/3.1415926535;
	if(x2-x1<0 && y2-y1>0)angle+=90;
	if(x2-x1<0 && y2-y1<=0)angle+=180;
	if(x2-x1>=0 && y2-y1<0)angle+=360;

	return angle;
}

double Block::right_top_angle()
{
	double x1 = geometry.x + geometry.w / 2;
	double y1 = geometry.y + geometry.h / 2;
	double x2 = geometry.x + geometry.w;
	double y2 = geometry.y;
	double angle = atan((y2-y1)/(x2-x1))*180/3.1415926535;
	if(x2-x1<0 && y2-y1>0)angle+=90;
	if(x2-x1<0 && y2-y1<=0)angle+=180;
	if(x2-x1>=0 && y2-y1<0)angle+=360;

	return angle;
}

double Block::left_bottom_angle()
{
	double x1 = geometry.x + geometry.w / 2;
	double y1 = geometry.y + geometry.h / 2;
	double x2 = geometry.x;
	double y2 = geometry.y + geometry.h;
	double angle = atan((y2-y1)/(x2-x1))*180/3.1415926535;
	if(x2-x1<0 && y2-y1>0)angle+=90;
	if(x2-x1<0 && y2-y1<=0)angle+=180;
	if(x2-x1>=0 && y2-y1<0)angle+=360;

	return angle;
}

double Block::left_top_angle()
{
	double x1 = geometry.x + geometry.w / 2;
	double y1 = geometry.y + geometry.h / 2;
	double x2 = geometry.x;
	double y2 = geometry.y;
	double angle = atan((y2-y1)/(x2-x1))*180/3.1415926535;
	if(x2-x1<0 && y2-y1>0)angle+=90;
	if(x2-x1<0 && y2-y1<=0)angle+=180;
	if(x2-x1>=0 && y2-y1<0)angle+=360;

	return angle;
}


double Block::angle_with_ball(Vec4f ball_geo)
{
	double x1 = geometry.x + geometry.w / 2;
	double y1 = geometry.y + geometry.h / 2;
	double x2 = ball_geo.x + ball_geo.w / 2;
	double y2 = ball_geo.y + ball_geo.h / 2;
	double angle = atan((y2-y1)/(x2-x1))*180/3.1415926535;
	if(x2-x1<0 && y2-y1>0)angle+=90;
	if(x2-x1<0 && y2-y1<=0)angle+=180;
	if(x2-x1>=0 && y2-y1<0)angle+=360;

	return angle;
}

bool Block::in_block(Vec2f point)
{
	return point.x >= left() && point.x <= right() && point.y >= top() && point.y <= bottom();
}
