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
#include <SDL_ttf.h>


using namespace std;

// Ԥ������
class Item;

class Game
{
public:
	Game(void);
	~Game(void);

	GameState game_state;
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


	// ��window����˵�
	SDL_Window *ptr;
	SDL_Renderer *renderer;
	SDL_Texture *textures[30];
	SDL_Texture *background;
	TTF_Font *font;

	bool game_started;

	// ��ʼ��
	void init();
	// �ػ�
	void draw();
	// ����
	void update();

	// ������ײ
	void handle_collision();
	// С����ש�����ײ��⣬�����Խ
	void handle_block_collision();
	// �������
	//void handle_hit(int i, Ball *ball, Block* block);

	// ��ȡͼ��
	SDL_Texture* load_image(string path);

	SDL_Texture* render_text(string message);

	SDL_Texture* text_texture;
	bool life_or_level_changed;
	SDL_Rect text_rect;

	bool quit;
	bool mouse_on_button;
	SDL_Rect option_rect;

	// ��ʱ��
	// ����
	const int FPS = 60;
	float frame_time;
	Uint32 previous_time;
	Uint32 current_time;
	float delta_time;
};

