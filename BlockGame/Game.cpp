#include "Game.h"
#include <algorithm>
#include <SDL_image.h>


Game::Game(void)
{
	// 初始化计时器变量
	frame_time = 0.0f;
	previous_time = 0;
	current_time = 0;
	delta_time = 0.0f;

	game_started = false;

	// 载入SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		cout << "SDL_Init Error: " << SDL_GetError() << endl;
		exit(1);
	}
	// 载入SDL_image和SDL_ttf
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
	TTF_Init();

	// 创建窗口
	ptr = SDL_CreateWindow("test", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	// 创建渲染器，其中SDL_RENDERER_PRESENTVSYNC可以自动控制帧率。
	renderer = SDL_CreateRenderer(ptr, -1, SDL_RENDERER_ACCELERATED/* | SDL_RENDERER_PRESENTVSYNC*/);

	// 清空材质列表
	for (int i = 0; i < 30; i++)
		textures[i] = nullptr;

	// 初始化字体
	font = nullptr;
	text_texture = nullptr;
	//font = TTF_OpenFont("res/SourceSansPro-Regular.ttf", 20);
	font = TTF_OpenFont("res/font.ttf", 18);
	// 初始化输出文字的矩形
	text_rect = { 880, 700, 130, 25 };
	quit = false;

	// 这个用来实现鼠标欧放到新游戏上变色的功能
	mouse_on_button = false;

	score = 0;
	life = 0;
	paused = false;
	// 从第一关开始
	level = 1;
	game_state = gs_main_menu;
	new_game();
}

Game::~Game(void)
{
	// 释放所有砖块
	for(list<Block*>::iterator block = blocks.begin(); block!=blocks.end(); ++block)
	{
		delete *block;
	}

	// 释放所有材质
	for (int i = 0; i < 20; i++)
	{
		if (textures[i])
			SDL_DestroyTexture(textures[i]);
	}
	TTF_CloseFont(font);
	TTF_Quit();
	IMG_Quit();
	// 释放渲染器和窗口
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(ptr);
	// 退出SDL
	SDL_Quit();
}

void Game::new_game()
{
	life = 3;
	// 踏板归位
	// 删除所有小球
	while(!balls.empty())
	{
		delete balls.front();
		balls.pop_front();
	}
	// 删除所有块
	for (list<Block*>::iterator block = blocks.begin(); block != blocks.end(); ++block)
	{
		delete *block;
	}
	blocks.clear();
	// 删除所有物品
	for (list<Item*>::iterator item = items.begin(); item != items.end(); ++item)
	{
		delete *item;
	}
	items.clear();
	// 删除所有动画
	for (list<Animation*>::iterator anim = animations.begin(); anim != animations.end(); ++anim)
	{
		delete *anim;
	}
	animations.clear();
	reset();

	// 初始化stick
    load_stage(level);
	// 掉落一个物品

	// 重设所有素材
	//set_textures(textures);

	// 重绘关卡号
	life_or_level_changed = true;
}

void Game::set_textures(SDL_Texture *textures[])
{
	// 设置板的材质
	paddle.set_texture(textures);


	for(list<Ball*>::iterator ball = balls.begin(); ball!=balls.end(); ++ball)
	{
		if(*ball)
		{
			(*ball)->set_texture(textures);
		}
	}
	// 设置砖块的材质
	for(list<Block*>::iterator block = blocks.begin(); block!=blocks.end(); ++block)
	{
		if(*block)
			(*block)->set_texture(textures);
	}
}


// 读取关卡
void Game::load_stage(int level)
{
	char stage_file_str[150] = { 0 };
	sprintf(stage_file_str, "stage/%d.txt", level);
	FILE *file = fopen(stage_file_str,"r");
	int type = 0, x_pos = 0, y_pos = 0;
	while(!feof(file))
	{
		fscanf(file, "%d %d %d\n", &type, &x_pos, &y_pos);
		switch(type)
		{
		case 1:
			break;
		case 2:
			blocks.push_back(new NormalBlock(x_pos, y_pos));
			break;
		case 3:
			blocks.push_back(new HardBlock(x_pos, y_pos));
			break;
		case 4:
			blocks.push_back(new SteelBlock(x_pos, y_pos));
			break;
		case 5:
			break;
		case 6:
			blocks.push_back(new ItemBlock(x_pos, y_pos));
			break;
		}

	}
}


// 有关道具的函数
void Game::improve_ball()
{
	Ball *temp = nullptr;
	for(list<Ball*>::iterator ball = balls.begin(); ball!=balls.end(); ++ball)
	{
		if(*ball)
		{
			// 这里把派生类当成基类了
			cout << typeid(*ball).name();
			if(dynamic_cast<NormalBall*>(*ball) != nullptr)
			{
				temp = new GravityBall();
				temp->set_geometry((*ball)->get_geometry());
				temp->speed = (*ball)->speed;
				temp->set_texture((*ball)->textures);
				temp->launch();
				delete (*ball);
				*ball = temp;
				// TODO: 这里以后可以用拷贝构造函数重写
			}
			else if(dynamic_cast<GravityBall*>(*ball) != nullptr)
			{
				temp = new SuperBall();
				temp->set_geometry((*ball)->get_geometry());
				temp->speed = (*ball)->speed;
				temp->set_texture((*ball)->textures);
				temp->launch();
				delete (*ball);
				*ball = temp;
			}
		}
	}
}

// 小球X3
void Game::triple_ball()
{
	list<Ball*> temp_list;
	for(list<Ball*>::iterator ball = balls.begin(); ball!=balls.end(); ++ball)
	{
		Ball *new_ball = (*ball)->clone();
		new_ball->speed.x += 2;
		temp_list.push_back(new_ball);
		new_ball = (*ball)->clone();
		new_ball->speed.x -= 2;
		temp_list.push_back(new_ball);
	}
	//sort(balls.begin(), balls.end());
	//sort(temp_list.begin(), temp_list.end());
	//balls.merge(temp_list);
	balls.splice(balls.end(), temp_list);
}

void Game::reset()
{
	life -= 1;
	life_or_level_changed = true;
	paddle.set_geometry(SCREEN_WIDTH/2, 680,100,10);
	paddle.size_maximized = false;
	// 装载一个小球
	balls.push_back(new NormalBall);
}

bool Game::is_clear()
{
	int result = true;
	for (list<Block*>::iterator block = blocks.begin(); block != blocks.end(); ++block)
	{
		if(*block)
		{
			if (dynamic_cast<SteelBlock*>(*block) != nullptr)
				continue;
			else
			{
				result = false;
				break;
			}
		}
	}
	return result;
}

void Game::init()
{
	textures[BACKGROUND] = IMG_LoadTexture(renderer, "res/bg.jpg");

	// 球
	textures[NORMAL_BALL] = load_image("res/normal_ball.png");
	textures[GRAVITY_BALL] = load_image("res/gravity_ball.png");
	textures[SUPER_BALL] = load_image("res/super_ball.png");
	// 板
	textures[STICK] = IMG_LoadTexture(renderer, "res/stick.bmp");

	// 砖块
	textures[NORMAL_BLOCK] = load_image("res/normal_block.png");
	textures[HARD_BLOCK] = load_image("res/hard_block.png");
	textures[HARD_BLOCK_CRACKED] = load_image("res/hard_block_cracked.png");
	textures[STEEL_BLOCK] = load_image("res/steel_block.png");
	textures[ITEM_BLOCK] = load_image("res/item_block.png");

	// 物品
	textures[ITEM_LONG_PADDLE] = load_image("res/item_long.jpg");
	textures[ITEM_IMPROVE_BALL] = load_image("res/item_improve_ball.jpg");
	textures[ITEM_TRIPLE_BALL] = load_image("res/item_triple_ball.png");
	textures[ITEM_ADD_LIFE] = load_image("res/item_add_life.png");

	// 动画
	textures[ANIMATION] = load_image("res/animation.png");
	// 暂停
	textures[PAUSE] = load_image("res/pause.png");
	// 标题
	textures[TITLE] = load_image("res/title.jpg");
	textures[NEW_GAME] = load_image("res/new_game.png");
	textures[NEW_GAME_2] = load_image("res/new_game2.png");
	textures[CHOOSE_STAGE] = load_image("res/choose_stage.png");

	this->background = textures[BACKGROUND];

	// 为游戏中的对象设置材质
	set_textures(textures);
}

void Game::draw()
{
	SDL_RenderPresent(renderer);
}

void Game::update()
{
	switch(game_state)
	{
	case gs_main_menu:
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, textures[TITLE], NULL, NULL);
		// SDL_Rect option_rect;
		if (!mouse_on_button)
		{
			SDL_QueryTexture(textures[NEW_GAME], NULL, NULL, &option_rect.w, &option_rect.h);
			option_rect.x = SCREEN_WIDTH / 2 - option_rect.w / 2;
			option_rect.y = 450;
			SDL_RenderCopy(renderer, textures[NEW_GAME], NULL, &option_rect);
		}
		else
		{
			SDL_QueryTexture(textures[NEW_GAME_2], NULL, NULL, &option_rect.w, &option_rect.h);
			option_rect.x = SCREEN_WIDTH / 2 - option_rect.w / 2;
			option_rect.y = 450;
			SDL_RenderCopy(renderer, textures[NEW_GAME_2], NULL, &option_rect);
		}

		/*SDL_QueryTexture(textures[CHOOSE_STAGE], NULL, NULL, &option_rect.w, &option_rect.h);
		option_rect.x = SCREEN_WIDTH / 2 - option_rect.w/2;
		option_rect.y = 520;
		SDL_RenderCopy(renderer, textures[CHOOSE_STAGE], NULL, &option_rect);*/
		SDL_RenderPresent(renderer);
		break;
	case gs_playing:
		//SDL_Event event;
			// 清空图像
			SDL_RenderClear(renderer);

			// 如果过关
			if (is_clear())
			{
				MessageBox(NULL, "请点击OK进入下一关...", "恭喜过关!", MB_OK);
				level++;
				new_game();
				set_textures(textures);
				return;
			}

			// 如果没有小球，且有生命，重置
			if (balls.size() == 0)
			{
				if (life == 0)
				{
					int choice = 0;
					choice = MessageBox(NULL, "再试一次？\n点是重试\n点否退出", "Game Over", MB_YESNO);
					// 判断用户的选择
					switch (choice)
					{
						// 用户选择是
					case 6:
						level = 1;
						new_game();
						set_textures(textures);
						return;
						break;
					case 7:
						//event.type = SDL_QUIT;
						// 事件会被复制一份到事件队列，这里可以不管
						// 收不到事件
						quit = true;
						//SDL_PushEvent(&event);
						return;
						break;
					case 2:
						break;
					}
				}
				else
				{
					reset();
					balls.front()->set_texture(textures);
				}

			}
			// 更新素材
			//game->set_textures(textures);

			// 背景
			SDL_RenderCopy(renderer, background, NULL, NULL);

			// 更新板状态
			paddle.update();
			SDL_RenderCopy(renderer, paddle.get_texture(), NULL, &paddle.get_geometry_sdl());

			// 更新所有球状态
			for (list<Ball*>::iterator it = balls.begin(); it != balls.end(); ++it)
			{
				if (*it)
				{
					// 如果小球未发射，跟着板走
					if (!(*it)->is_launched())
						(*it)->set_pos(paddle.get_pos().x + paddle.get_geometry().w / 2 - (*it)->get_geometry().w / 2, paddle.get_pos().y - (*it)->get_geometry().h);
					// 分两次更新小球位置，以达到增加碰撞采样点
					(*it)->update();
					handle_collision();
					// 如果小球尚未坠毁
					if (!(*it)->crashed)
						(*it)->update();
				}
				SDL_RenderCopy(renderer, (*it)->get_texture(), NULL, &(*it)->get_geometry_sdl());
			}

			// 处理碰撞
			handle_collision();

			// 消除坠毁的小球
			for (list<Ball*>::iterator ball = balls.begin(); ball != balls.end(); )
			{
				if ((*ball)->crashed)
				{
					delete (*ball);
					// 坠毁
					ball = balls.erase(ball);

				}
				else
					++ball;
			}


			// 所有砖块更新
			for (list<Block*>::iterator block = blocks.begin(); block != blocks.end(); ++block)
			{
				if (*block)
				{
					SDL_RenderCopy(renderer, (*block)->get_texture(), NULL, &(*block)->get_geometry_sdl());
				}
			}

			// 获取帧数，更新动画信息
			//int frame = int(((SDL_GetTicks() / 10) % 30));
			for (list<Animation*>::iterator anim = animations.begin(); anim != animations.end(); )
			{
				if (*anim)
				{
					(*anim)->update2();
					SDL_RenderCopy(renderer, (*anim)->texture, &(*anim)->source_rect, &(*anim)->target_rect);
				}
				// 删除播放完的
				if ((*anim)->frame == 27)
					anim = animations.erase(anim);
				else
					++anim;
			}

			// 更新所有物品状态
			for (list<Item*>::iterator item = items.begin(); item != items.end(); ++item)
			{
				if (*item)
				{
					(*item)->update();
				}
				SDL_RenderCopy(renderer, (*item)->get_texture(), NULL, &(*item)->get_geometry_sdl());
			}

			// 渲染生命数和关卡号
			if (life_or_level_changed)
			{
				SDL_DestroyTexture(text_texture);
				text_texture = nullptr;
				char temp_str[50];
				sprintf(temp_str, "LEVEL: %d LIFE: %d", level, life);
				text_texture = render_text(temp_str);
				life_or_level_changed = false;
			}


			SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);

		break;
	case gs_paused:
		SDL_Rect temp = { SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 50, 100, 100 };
		SDL_RenderCopy(renderer, textures[PAUSE], NULL, &temp);
		break;
	}
}

void Game::handle_collision()
{
	// 如果已经没有小球了，就免了
	if (balls.size() == 0)
		return;
	for (list<Ball*>::iterator ball = balls.begin(); ball != balls.end(); )
	{
		// 墙壁
		if ((*ball)->speed.x > 0 && (*ball)->get_geometry().x >= SCREEN_WIDTH - (*ball)->get_geometry().w)
			(*ball)->speed.x *= -1;
		if ((*ball)->speed.x < 0 && (*ball)->get_geometry().x <= 0)
			(*ball)->speed.x *= -1;
		if ((*ball)->speed.y > 0 && (*ball)->get_geometry().y >= SCREEN_HEIGHT - (*ball)->get_geometry().h)
		{
			// 将小球标记为坠毁
			(*ball)->crashed = true;
			// 坠毁
			//delete (*ball);
			//ball = game->balls.erase(ball);
			++ball;
			continue;
		}

		if ((*ball)->speed.y < 0 && (*ball)->get_geometry().y <= 0)
			(*ball)->speed.y *= -1;
		if ((*ball)->speed.y > 0 && (*ball)->get_geometry().y >= paddle.get_geometry().y - (*ball)->get_geometry().h && (*ball)->get_geometry().x + (*ball)->get_geometry().w / 2 > paddle.get_geometry().x && (*ball)->get_geometry().x + (*ball)->get_geometry().w / 2 < paddle.get_geometry().x + paddle.get_geometry().w)
		{
			(*ball)->speed.y *= -1;
			// 板的速度影响小球的速度
			(*ball)->speed.x += paddle.speed.x / 3;
			/*// 小球水平速度最高正负12
			if(abs((*ball)->speed.x)>12)
			(*ball)->speed.x = (*ball)->speed.x / abs((*ball)->speed.x) *12;*/
			// 打在板的不同位置会对小球水平速度造成不同的影响
			/*(*ball)->speed.x += ((*ball)->get_pos().x-game->stick.get_pos().x - game->stick.get_geometry().w / 2)
			/ (game->stick.get_geometry().w/2) * 12;*/

			(*ball)->speed.x = (double)((*ball)->get_pos().x + (*ball)->get_geometry().w / 2 - paddle.get_pos().x - paddle.get_geometry().w / 2)
				/ (double)(paddle.get_geometry().w / 2) * 8;
			// 小球水平速度最高正负12
			/*if(abs((*ball)->speed.x)>12)
			(*ball)->speed.x = (*ball)->speed.x / abs((*ball)->speed.x) *12;*/
		}

		// 砖块
		bool hit = false;
		for (list<Block*>::iterator block = blocks.begin(); block != blocks.end();)
		{

			hit = false;
			if (*block)
			{
				// 如果小球有像素位于砖块左右边界之内
				// 关于角度，先把等于也放在碰撞上下边界中
				// 碰撞砖块上下边界
				// 判断小球是否与砖块发生了接触
				// 第一行限定小球与砖块碰撞以小球边界为基准还是以小球中线为基准
				if (((*ball)->get_h_mid() >= (*block)->left() && (*ball)->get_h_mid() <= (*block)->right())
					// 第二、三行判断小球是否处于砖块上方左右范围内
					&& (((*block)->angle_with_ball((*ball)->get_geometry()) <= (*block)->right_top_angle()
						&& (*block)->angle_with_ball((*ball)->get_geometry()) >= (*block)->left_top_angle())
						//第四、五行判断小球是否处于砖块下方左右范围内
						|| ((*block)->angle_with_ball((*ball)->get_geometry()) <= (*block)->left_bottom_angle()
							&& (*block)->angle_with_ball((*ball)->get_geometry()) >= (*block)->right_bottom_angle())))
				{
					// 碰撞砖块下界
					if (((*ball)->top() < (*block)->bottom() && (*ball)->bottom() > (*block)->bottom())/* || (*ball)->last_pos.y > (*block)->bottom()*/)
					{
						(*ball)->hit(BOTTOM);
						(*block)->hit_by((*ball), &items);
					}

					// 碰撞砖块上界
					else if (((*ball)->bottom() > (*block)->top() && (*ball)->top() < (*block)->top())/* || (*ball)->last_pos.y < (*block)->top()*/)
					{
						(*ball)->hit(TOP);
						(*block)->hit_by((*ball), &items);
					}
				}
				// 碰撞砖块左右边界
				// 如果小球有像素位于砖块上下边界之内
				else if ((*ball)->get_v_mid() >= (*block)->top() && (*ball)->get_v_mid() <= (*block)->bottom())
				{

					// 碰撞砖块左界

					if ((*ball)->right() > (*block)->left() && (*ball)->left() < (*block)->left())
					{
						(*ball)->hit(LEFT);
						(*block)->hit_by((*ball), &items);
					}
					// 碰撞砖块右界
					else if ((*ball)->left() < (*block)->right() && (*ball)->right() > (*block)->right())
					{
						(*ball)->hit(RIGHT);
						(*block)->hit_by((*ball), &items);
					}
				}
				// 删除生命值小于等于0 的砖块
				// 如果砖块被破坏，创建一个动画
				if ((*block)->health <= 0)
				{
					// 在砖块的位置创建一个动画
					animations.push_back(new Animation(textures[ANIMATION],
						(*block)->get_h_mid() - 35,
						(*block)->get_v_mid() - 17));
					delete (*block);
					*block = nullptr;
					block = blocks.erase(block);
					hit = true;
				}
				if (!hit)
					++block;
			}
		}
		++ball;
	}
	// 道具
	for (list<Item*>::iterator item = items.begin(); item != items.end();)
	{
		// 如果道具被吃到
		if ((*item)->speed.y > 0 &&
			(*item)->get_geometry().y >= paddle.get_geometry().y - (*item)->get_geometry().h &&
			(*item)->get_geometry().x + (*item)->get_geometry().w / 2 > paddle.get_geometry().x &&
			(*item)->get_geometry().x + (*item)->get_geometry().w / 2 < paddle.right() &&
			(*item)->top() <= paddle.bottom())
		{
			// 判断类型，发动效果
			switch ((*item)->type)
			{
			case ITEM_LONG_PADDLE:
				paddle.size_up();
				(*item)->gotten = true;
				break;
			case ITEM_IMPROVE_BALL:
				improve_ball();
				(*item)->gotten = true;
				break;
			case ITEM_TRIPLE_BALL:
				triple_ball();
				(*item)->gotten = true;
				break;
			case ITEM_ADD_LIFE:
				life++;
				life_or_level_changed = true;
				(*item)->gotten = true;
				break;
			}
		}
		// 如果没吃到
		else if ((*item)->bottom() == SCREEN_HEIGHT)
			(*item)->gotten = true;
		// 删除生命值小于等于0 的砖块
		if ((*item)->gotten)
		{
			delete (*item);
			*item = nullptr;
			item = items.erase(item);
			continue;
		}
		++item;
	}
}

SDL_Texture* Game::load_image(string path)
{
	SDL_Surface *picture = nullptr;

	//IMG_Init(IMG_INIT_PNG);
	picture = IMG_Load(path.c_str());
	Uint32 colorkey = SDL_MapRGB(picture->format, 69, 69, 69);
	SDL_SetColorKey(picture, SDL_TRUE, colorkey);
	SDL_Texture *result = SDL_CreateTextureFromSurface(renderer, picture);
	SDL_FreeSurface(picture);
	return result;
}

SDL_Texture* Game::render_text(string message)
{
	SDL_Color color = { 255, 0, 0 };
	SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
	//Clean up unneeded stuff
	SDL_FreeSurface(surf);

	return texture;
}

