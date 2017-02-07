#pragma once
#include "Paddle.h"
#include "Ball.h"
#include "Block.h"
#include <list>
#include <iostream>
#include "common_def.h"
#include "NormalBlock.h"
#include "NormalBall.h"
#include "GravityBall.h"
#include "SuperBall.h"
#include "HardBlock.h"
#include "SteelBlock.h"
#include "ItemBlock.h"
#include "Item.h"
#include "Animation.h"
#include <SDL.h>


using namespace std;

// 预先声明
class Item;

class Game
{
public:
	Game(void);
	~Game(void);
	

	// 分数、命数
	int score;
	int life;
	bool paused;
	int level;
	// 板
	Paddle paddle;
	// 球
	list<Ball*> balls;
	//Ball *balls[3];
	// 砖块
	//Block **blocks;
	list<Block*> blocks;
	// 道具
	list<Item*> items;
	// 动画
	list<Animation*> animations;

	// 新游戏
	void new_game();
	// 读取关卡
	void load_stage(int level);
	// 装载材质
	void set_textures(SDL_Texture *textures[]);
	// 有关道具的函数
	void improve_ball();
	void triple_ball();
	// 重置滑板
	void reset();

	bool is_clear();
};

