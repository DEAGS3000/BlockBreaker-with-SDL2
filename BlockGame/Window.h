#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Game.h"


class Window
{
public:

	//const int SCREEN_WIDTH;
	//const int SCREEN_HEIGHT;

	Window(void);
	~Window(void);

	SDL_Window *ptr;
	SDL_Renderer *renderer;

	SDL_Texture *textures[30];
	SDL_Texture *background;

	Game *game;

	TTF_Font *font;

	bool game_started;

	// 初始化
	void init();
	// 重绘
	void refresh();
	// 更新
	void update();
	
	// 处理碰撞
	void handle_collision();
	// 处理击中
	//void handle_hit(int i, Ball *ball, Block* block);

	// 读取图像
	SDL_Texture* load_image(string path);

	SDL_Texture* render_text(string message);

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

