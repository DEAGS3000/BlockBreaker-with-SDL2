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

	// ��ʼ��
	void init();
	// �ػ�
	void refresh();
	// ����
	void update();
	
	// ������ײ
	void handle_collision();
	// �������
	//void handle_hit(int i, Ball *ball, Block* block);

	// ��ȡͼ��
	SDL_Texture* load_image(string path);

	SDL_Texture* render_text(string message);

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

