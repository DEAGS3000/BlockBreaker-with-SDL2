#include <SDL.h>
#include <SDL_image.h>
#include <cstdlib>
#include<Windows.h>
#include <iostream>
#include "Window.h"

#define TIME_PER_FRAME 


using namespace std;

int main(int argc, char *argv[])
{
	// 置随机种子
	srand(static_cast<unsigned int>(GetTickCount()));

	// 载入准备部分开始
	// 创建窗口
	Window win;
	win.init();
	//记录窗口大小


	// 创建事件结构体
	SDL_Event e;
	//主循环，直到SDL事件e捕获到点击关闭按钮才跳出并销毁窗体
	while(!win.quit)
	{
		// 计时器相关
		win.previous_time = win.current_time;
		win.current_time = SDL_GetTicks();
		win.delta_time = (win.current_time - win.previous_time) / 1000.0f;

		if(SDL_PollEvent(&e))
		{

			switch(e.type)
			{
			case SDL_QUIT:
				win.quit = true;
				break;
			case SDL_KEYDOWN:
				switch(e.key.keysym.sym)
				{
					// 左键按下
				case SDLK_LEFT:
					win.game->paddle.speed.x = -12;

					//btn_released = false;
					break;
				case SDLK_RIGHT:
					win.game->paddle.speed.x = 12;

					//btn_released = false;
					break;
				case SDLK_UP:
					// 发射小球
					for(list<Ball*>::iterator it = win.game->balls.begin(); it!=win.game->balls.end(); ++it)
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
					if(win.game->paused)
						win.game->paused = false;
					else
						win.game->paused = true;
					break;
				}
				break;
			case SDL_KEYUP:
				switch(e.key.keysym.sym)
				{
				case SDLK_LEFT:
					win.game->paddle.speed.x = 0;

					//btn_released = true;
					break;
				case SDLK_RIGHT:
					win.game->paddle.speed.x = 0;
					//btn_released = true;
					break;
				}
				break;
			case SDL_MOUSEMOTION:
				if(!win.game_started)
				{
					if (e.button.x > win.option_rect.x && e.button.x < win.option_rect.x + win.option_rect.w
						&& e.button.y > win.option_rect.y && e.button.y < win.option_rect.y + win.option_rect.h)
						win.mouse_on_button = true;
					else
						win.mouse_on_button = false;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				// 如果左键按下
				if(e.button.button==SDL_BUTTON_LEFT)
				{

					if (e.button.x > win.option_rect.x && e.button.x < win.option_rect.x + win.option_rect.w
						&& e.button.y > win.option_rect.y && e.button.y < win.option_rect.y + win.option_rect.h)
						win.game_started = true;
				}
				break;
			default:
				break;
			}
			
		}

		win.frame_time += win.delta_time;
		
		// 更新并渲染
		// 如果frame_time已经够了，就清空并更新
		if(win.frame_time > 0.01666f)
		{
			
			win.frame_time = 0.0f;
			win.update();
			win.refresh();
		}
		//cout << win.frame_time << endl;
		//win.update();


		
	}
	

	return 0;
}