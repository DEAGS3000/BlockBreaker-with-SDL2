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

// Ԥ������
class Item;

class Game
{
public:
	Game(void);
	~Game(void);
	

	// ����������
	int score;
	int life;
	bool paused;
	int level;
	// ��
	Paddle paddle;
	// ��
	list<Ball*> balls;
	//Ball *balls[3];
	// ש��
	//Block **blocks;
	list<Block*> blocks;
	// ����
	list<Item*> items;
	// ����
	list<Animation*> animations;

	// ����Ϸ
	void new_game();
	// ��ȡ�ؿ�
	void load_stage(int level);
	// װ�ز���
	void set_textures(SDL_Texture *textures[]);
	// �йص��ߵĺ���
	void improve_ball();
	void triple_ball();
	// ���û���
	void reset();

	bool is_clear();
};

