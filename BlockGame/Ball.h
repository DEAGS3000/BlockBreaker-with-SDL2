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
	// 击中，包括反弹处理
	virtual void hit(int direction);
	//void update();
	void launch();
	bool is_launched();
	double radius();
	void update(float delta_time = 1.0f);
// 破坏力
	int damage;
	// 克隆
	virtual Ball* clone() = 0;
	// 小球是否已坠毁
	bool crashed;
	Vec2f last_pos;
protected:
	bool launched;
};

