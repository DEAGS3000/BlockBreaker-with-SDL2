#pragma once
#include "common_def.h"
#include "graphicitem.h"
#include "Ball.h"
#include "Item.h"
#include <list>

using namespace std;

// ����ͷ�ļ�����������ᵼ��δ�������
// ��Ҫ������һ������������
class Ball;

class Block :
	public GraphicItem
{
public:
	Block(void);
	~Block(void);

	// ש�鱻����
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

