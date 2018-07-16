#pragma once
#include "common_def.h"
#include "graphicitem.h"
#include "Block.h"

class Block;

class Ball :
	public GraphicItem
{
public:
	Ball(void);
	Ball(Ball& ball);
	~Ball(void);
	// ���У�������������
	virtual void hit(int direction);
	//void update();
	void launch();
	bool is_launched();
	double radius();
	void update(float delta_time = 1.0f);
// �ƻ���
	int damage;
	// ��¡
	virtual Ball* clone() = 0;
	// С���Ƿ���׹��
	bool crashed;
	Vec2f last_pos;
protected:
	bool launched;
};

