#include <SDL.h>
#include <SDL_image.h>
#include <cstdlib>
#include<Windows.h>
#include <iostream>
#include "Game.h"

#define TIME_PER_FRAME 


using namespace std;

int main(int argc, char *argv[])
{
	// 置随机种子
	srand(static_cast<unsigned int>(GetTickCount()));

	// 载入准备部分开始
	// 创建窗口
	Game game;
	game.init();
	//记录窗口大小


	// 创建事件结构体
	SDL_Event e;
	//主循环，直到SDL事件e捕获到点击关闭按钮才跳出并销毁窗体
	while(!game.quit)
	{
		// 计时器相关
		game.previous_time = game.current_time;
		game.current_time = SDL_GetTicks();
		game.delta_time = (game.current_time - game.previous_time) / 1000.0f;

		if(SDL_PollEvent(&e))
		{

			switch(e.type)
			{
			case SDL_QUIT:
				game.quit = true;
				break;
			case SDL_KEYDOWN:
				switch(e.key.keysym.sym)
				{
					// 左键按下
				case SDLK_LEFT:
					game.paddle.speed.x = -12;

					//btn_released = false;
					break;
				case SDLK_RIGHT:
					game.paddle.speed.x = 12;

					//btn_released = false;
					break;
				case SDLK_UP:
					// 发射小球
					for(list<Ball*>::iterator it = game.balls.begin(); it!=game.balls.end(); ++it)
					{
						// 每次只发射一个
						if(!(*it)->is_launched())
						{
							(*it)->launch();
							break;
						}
					}
					break;
				case SDLK_ESCAPE:
					if (game.game_state == gs_playing)
						game.game_state = gs_paused;
					if (game.game_state == gs_paused)
						game.game_state = gs_playing;
					break;
				}
				break;
			case SDL_KEYUP:
				switch(e.key.keysym.sym)
				{
				case SDLK_LEFT:
					game.paddle.speed.x = 0;

					//btn_released = true;
					break;
				case SDLK_RIGHT:
					game.paddle.speed.x = 0;
					//btn_released = true;
					break;
				}
				break;
			case SDL_MOUSEMOTION:
				if(game.game_state==gs_main_menu)
				{
					if (e.button.x > game.option_rect.x && e.button.x < game.option_rect.x + game.option_rect.w
						&& e.button.y > game.option_rect.y && e.button.y < game.option_rect.y + game.option_rect.h)
						game.mouse_on_button = true;
					else
						game.mouse_on_button = false;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				// 如果左键按下
				if(e.button.button==SDL_BUTTON_LEFT)
				{

					if (e.button.x > game.option_rect.x && e.button.x < game.option_rect.x + game.option_rect.w
						&& e.button.y > game.option_rect.y && e.button.y < game.option_rect.y + game.option_rect.h)
						//game.game_started = true;
						game.game_state = gs_playing;
				}
				break;
			default:
				break;
			}
		}

		game.frame_time += game.delta_time;

		// 更新并渲染
		// 如果frame_time已经够了，就清空并更新
		if(game.frame_time > 0.01666f)
		{

			game.frame_time = 0.0f;
			game.update();
			game.draw();
		}
		//cout << win.frame_time << endl;
		//win.update();


		
	}
	

	return 0;
}