#pragma once
#include "common_def.h"
#include "graphicitem.h"
#include "Ball.h"
#include "Item.h"
#include <list>

using namespace std;

// 两个头文件互相包含，会导致未定义错误
// 需要对其中一个类先做声明
class Ball;

class Block :
	public GraphicItem
{
public:
	Block(void);
	~Block(void);

	// 砖块被击中
	virtual void hit_by(Ball *ball, list<Item*> *l);
	double right_bottom_angle();
	double left_bottom_angle();
	double right_top_angle();
	double left_top_angle();
	double angle_with_ball(Vec4f ball_geo);
	bool in_block(Vec2f point);
int health;
	int score;
protected:

};

