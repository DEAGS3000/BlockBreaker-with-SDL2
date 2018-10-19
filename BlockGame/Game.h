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

// 预先声明
class Item;

class Game
{
public:
	Game(void);
	~Game(void);

	GameState game_state;
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


	// 从window类搬运的
	SDL_Window *ptr;
	SDL_Renderer *renderer;
	SDL_Texture *textures[30];
	SDL_Texture *background;
	TTF_Font *font;

	bool game_started;

	// 初始化
	void init();
	// 重绘
	void draw();
	// 更新
	void update();

	// 处理碰撞
	void handle_collision();
	// 小球与砖块的碰撞检测，避免飞越
	void handle_block_collision();
	// 处理击中
	//void handle_hit(int i, Ball *ball, Block* block);

	// 读取图像
	SDL_Texture* load_image(string path);

	SDL_Texture* render_text(string message);

	SDL_Texture* text_texture;
	bool life_or_level_changed;
	SDL_Rect text_rect;

	bool quit;
	bool mouse_on_button;
	SDL_Rect option_rect;

	// 计时器
	// 测试
	const int FPS = 60;
	float frame_time;
	Uint32 previous_time;
	Uint32 current_time;
	float delta_time;
};

