#include "Game.h"
#include <algorithm>
#include <SDL_image.h>


Game::Game(void)
{
	// ��ʼ����ʱ������
	frame_time = 0.0f;
	previous_time = 0;
	current_time = 0;
	delta_time = 0.0f;

	game_started = false;

	// ����SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		cout << "SDL_Init Error: " << SDL_GetError() << endl;
		exit(1);
	}
	// ����SDL_image��SDL_ttf
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
	TTF_Init();

	// ��������
	ptr = SDL_CreateWindow("test", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	// ������Ⱦ��������SDL_RENDERER_PRESENTVSYNC�����Զ�����֡�ʡ�
	renderer = SDL_CreateRenderer(ptr, -1, SDL_RENDERER_ACCELERATED/* | SDL_RENDERER_PRESENTVSYNC*/);

	// ��ղ����б�
	for (int i = 0; i < 30; i++)
		textures[i] = nullptr;

	// ��ʼ������
	font = nullptr;
	text_texture = nullptr;
	//font = TTF_OpenFont("res/SourceSansPro-Regular.ttf", 20);
	font = TTF_OpenFont("res/font.ttf", 18);
	// ��ʼ��������ֵľ���
	text_rect = { 880, 700, 130, 25 };
	quit = false;

	// �������ʵ�����ŷ�ŵ�����Ϸ�ϱ�ɫ�Ĺ���
	mouse_on_button = false;

	score = 0;
	life = 0;
	paused = false;
	// �ӵ�һ�ؿ�ʼ
	level = 1;
	game_state = gs_main_menu;
	new_game();
}

Game::~Game(void)
{
	// �ͷ�����ש��
	for(list<Block*>::iterator block = blocks.begin(); block!=blocks.end(); ++block)
	{
		delete *block;
	}

	// �ͷ����в���
	for (int i = 0; i < 20; i++)
	{
		if (textures[i])
			SDL_DestroyTexture(textures[i]);
	}
	TTF_CloseFont(font);
	TTF_Quit();
	IMG_Quit();
	// �ͷ���Ⱦ���ʹ���
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(ptr);
	// �˳�SDL
	SDL_Quit();
}

void Game::new_game()
{
	life = 3;
	// ̤���λ
	// ɾ������С��
	while(!balls.empty())
	{
		delete balls.front();
		balls.pop_front();
	}
	// ɾ�����п�
	for (list<Block*>::iterator block = blocks.begin(); block != blocks.end(); ++block)
	{
		delete *block;
	}
	blocks.clear();
	// ɾ��������Ʒ
	for (list<Item*>::iterator item = items.begin(); item != items.end(); ++item)
	{
		delete *item;
	}
	items.clear();
	// ɾ�����ж���
	for (list<Animation*>::iterator anim = animations.begin(); anim != animations.end(); ++anim)
	{
		delete *anim;
	}
	animations.clear();
	reset();

	// ��ʼ��stick
    load_stage(level);
	// ����һ����Ʒ

	// ���������ز�
	//set_textures(textures);

	// �ػ�ؿ���
	life_or_level_changed = true;
}

void Game::set_textures(SDL_Texture *textures[])
{
	// ���ð�Ĳ���
	paddle.set_texture(textures);


	for(list<Ball*>::iterator ball = balls.begin(); ball!=balls.end(); ++ball)
	{
		if(*ball)
		{
			(*ball)->set_texture(textures);
		}
	}
	// ����ש��Ĳ���
	for(list<Block*>::iterator block = blocks.begin(); block!=blocks.end(); ++block)
	{
		if(*block)
			(*block)->set_texture(textures);
	}
}


// ��ȡ�ؿ�
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


// �йص��ߵĺ���
void Game::improve_ball()
{
	Ball *temp = nullptr;
	for(list<Ball*>::iterator ball = balls.begin(); ball!=balls.end(); ++ball)
	{
		if(*ball)
		{
			// ����������൱�ɻ�����
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
				// TODO: �����Ժ�����ÿ������캯����д
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

// С��X3
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
	// װ��һ��С��
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

	// ��
	textures[NORMAL_BALL] = load_image("res/normal_ball.png");
	textures[GRAVITY_BALL] = load_image("res/gravity_ball.png");
	textures[SUPER_BALL] = load_image("res/super_ball.png");
	// ��
	textures[STICK] = IMG_LoadTexture(renderer, "res/stick.bmp");

	// ש��
	textures[NORMAL_BLOCK] = load_image("res/normal_block.png");
	textures[HARD_BLOCK] = load_image("res/hard_block.png");
	textures[HARD_BLOCK_CRACKED] = load_image("res/hard_block_cracked.png");
	textures[STEEL_BLOCK] = load_image("res/steel_block.png");
	textures[ITEM_BLOCK] = load_image("res/item_block.png");

	// ��Ʒ
	textures[ITEM_LONG_PADDLE] = load_image("res/item_long.jpg");
	textures[ITEM_IMPROVE_BALL] = load_image("res/item_improve_ball.jpg");
	textures[ITEM_TRIPLE_BALL] = load_image("res/item_triple_ball.png");
	textures[ITEM_ADD_LIFE] = load_image("res/item_add_life.png");

	// ����
	textures[ANIMATION] = load_image("res/animation.png");
	// ��ͣ
	textures[PAUSE] = load_image("res/pause.png");
	// ����
	textures[TITLE] = load_image("res/title.jpg");
	textures[NEW_GAME] = load_image("res/new_game.png");
	textures[NEW_GAME_2] = load_image("res/new_game2.png");
	textures[CHOOSE_STAGE] = load_image("res/choose_stage.png");

	this->background = textures[BACKGROUND];

	// Ϊ��Ϸ�еĶ������ò���
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
			// ���ͼ��
			SDL_RenderClear(renderer);

			// �������
			if (is_clear())
			{
				MessageBox(NULL, "����OK������һ��...", "��ϲ����!", MB_OK);
				level++;
				new_game();
				set_textures(textures);
				return;
			}

			// ���û��С����������������
			if (balls.size() == 0)
			{
				if (life == 0)
				{
					int choice = 0;
					choice = MessageBox(NULL, "����һ�Σ�\n��������\n����˳�", "Game Over", MB_YESNO);
					// �ж��û���ѡ��
					switch (choice)
					{
						// �û�ѡ����
					case 6:
						level = 1;
						new_game();
						set_textures(textures);
						return;
						break;
					case 7:
						//event.type = SDL_QUIT;
						// �¼��ᱻ����һ�ݵ��¼����У�������Բ���
						// �ղ����¼�
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
			// �����ز�
			//game->set_textures(textures);

			// ����
			SDL_RenderCopy(renderer, background, NULL, NULL);

			// ���°�״̬
			paddle.update();
			SDL_RenderCopy(renderer, paddle.get_texture(), NULL, &paddle.get_geometry_sdl());

			// ����������״̬
			for (list<Ball*>::iterator it = balls.begin(); it != balls.end(); ++it)
			{
				if (*it)
				{
					// ���С��δ���䣬���Ű���
					if (!(*it)->is_launched())
						(*it)->set_pos(paddle.get_pos().x + paddle.get_geometry().w / 2 - (*it)->get_geometry().w / 2, paddle.get_pos().y - (*it)->get_geometry().h);
					// �����θ���С��λ�ã��Դﵽ������ײ������
					(*it)->update();
					handle_collision();
					// ���С����δ׹��
					if (!(*it)->crashed)
						(*it)->update();
				}
				SDL_RenderCopy(renderer, (*it)->get_texture(), NULL, &(*it)->get_geometry_sdl());
			}

			// ������ײ
			handle_collision();

			// ����׹�ٵ�С��
			for (list<Ball*>::iterator ball = balls.begin(); ball != balls.end(); )
			{
				if ((*ball)->crashed)
				{
					delete (*ball);
					// ׹��
					ball = balls.erase(ball);

				}
				else
					++ball;
			}


			// ����ש�����
			for (list<Block*>::iterator block = blocks.begin(); block != blocks.end(); ++block)
			{
				if (*block)
				{
					SDL_RenderCopy(renderer, (*block)->get_texture(), NULL, &(*block)->get_geometry_sdl());
				}
			}

			// ��ȡ֡�������¶�����Ϣ
			//int frame = int(((SDL_GetTicks() / 10) % 30));
			for (list<Animation*>::iterator anim = animations.begin(); anim != animations.end(); )
			{
				if (*anim)
				{
					(*anim)->update2();
					SDL_RenderCopy(renderer, (*anim)->texture, &(*anim)->source_rect, &(*anim)->target_rect);
				}
				// ɾ���������
				if ((*anim)->frame == 27)
					anim = animations.erase(anim);
				else
					++anim;
			}

			// ����������Ʒ״̬
			for (list<Item*>::iterator item = items.begin(); item != items.end(); ++item)
			{
				if (*item)
				{
					(*item)->update();
				}
				SDL_RenderCopy(renderer, (*item)->get_texture(), NULL, &(*item)->get_geometry_sdl());
			}

			// ��Ⱦ�������͹ؿ���
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
	// ����Ѿ�û��С���ˣ�������
	if (balls.size() == 0)
		return;
	for (list<Ball*>::iterator ball = balls.begin(); ball != balls.end(); )
	{
		// ǽ��
		if ((*ball)->speed.x > 0 && (*ball)->get_geometry().x >= SCREEN_WIDTH - (*ball)->get_geometry().w)
			(*ball)->speed.x *= -1;
		if ((*ball)->speed.x < 0 && (*ball)->get_geometry().x <= 0)
			(*ball)->speed.x *= -1;
		if ((*ball)->speed.y > 0 && (*ball)->get_geometry().y >= SCREEN_HEIGHT - (*ball)->get_geometry().h)
		{
			// ��С����Ϊ׹��
			(*ball)->crashed = true;
			// ׹��
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
			// ����ٶ�Ӱ��С����ٶ�
			(*ball)->speed.x += paddle.speed.x / 3;
			/*// С��ˮƽ�ٶ��������12
			if(abs((*ball)->speed.x)>12)
			(*ball)->speed.x = (*ball)->speed.x / abs((*ball)->speed.x) *12;*/
			// ���ڰ�Ĳ�ͬλ�û��С��ˮƽ�ٶ���ɲ�ͬ��Ӱ��
			/*(*ball)->speed.x += ((*ball)->get_pos().x-game->stick.get_pos().x - game->stick.get_geometry().w / 2)
			/ (game->stick.get_geometry().w/2) * 12;*/

			(*ball)->speed.x = (double)((*ball)->get_pos().x + (*ball)->get_geometry().w / 2 - paddle.get_pos().x - paddle.get_geometry().w / 2)
				/ (double)(paddle.get_geometry().w / 2) * 8;
			// С��ˮƽ�ٶ��������12
			/*if(abs((*ball)->speed.x)>12)
			(*ball)->speed.x = (*ball)->speed.x / abs((*ball)->speed.x) *12;*/
		}

		// ש��
		bool hit = false;
		for (list<Block*>::iterator block = blocks.begin(); block != blocks.end();)
		{

			hit = false;
			if (*block)
			{
				// ���С��������λ��ש�����ұ߽�֮��
				// ���ڽǶȣ��Ȱѵ���Ҳ������ײ���±߽���
				// ��ײש�����±߽�
				// �ж�С���Ƿ���ש�鷢���˽Ӵ�
				// ��һ���޶�С����ש����ײ��С��߽�Ϊ��׼������С������Ϊ��׼
				if (((*ball)->get_h_mid() >= (*block)->left() && (*ball)->get_h_mid() <= (*block)->right())
					// �ڶ��������ж�С���Ƿ���ש���Ϸ����ҷ�Χ��
					&& (((*block)->angle_with_ball((*ball)->get_geometry()) <= (*block)->right_top_angle()
						&& (*block)->angle_with_ball((*ball)->get_geometry()) >= (*block)->left_top_angle())
						//���ġ������ж�С���Ƿ���ש���·����ҷ�Χ��
						|| ((*block)->angle_with_ball((*ball)->get_geometry()) <= (*block)->left_bottom_angle()
							&& (*block)->angle_with_ball((*ball)->get_geometry()) >= (*block)->right_bottom_angle())))
				{
					// ��ײש���½�
					if (((*ball)->top() < (*block)->bottom() && (*ball)->bottom() > (*block)->bottom())/* || (*ball)->last_pos.y > (*block)->bottom()*/)
					{
						(*ball)->hit(BOTTOM);
						(*block)->hit_by((*ball), &items);
					}

					// ��ײש���Ͻ�
					else if (((*ball)->bottom() > (*block)->top() && (*ball)->top() < (*block)->top())/* || (*ball)->last_pos.y < (*block)->top()*/)
					{
						(*ball)->hit(TOP);
						(*block)->hit_by((*ball), &items);
					}
				}
				// ��ײש�����ұ߽�
				// ���С��������λ��ש�����±߽�֮��
				else if ((*ball)->get_v_mid() >= (*block)->top() && (*ball)->get_v_mid() <= (*block)->bottom())
				{

					// ��ײש�����

					if ((*ball)->right() > (*block)->left() && (*ball)->left() < (*block)->left())
					{
						(*ball)->hit(LEFT);
						(*block)->hit_by((*ball), &items);
					}
					// ��ײש���ҽ�
					else if ((*ball)->left() < (*block)->right() && (*ball)->right() > (*block)->right())
					{
						(*ball)->hit(RIGHT);
						(*block)->hit_by((*ball), &items);
					}
				}
				// ɾ������ֵС�ڵ���0 ��ש��
				// ���ש�鱻�ƻ�������һ������
				if ((*block)->health <= 0)
				{
					// ��ש���λ�ô���һ������
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
	// ����
	for (list<Item*>::iterator item = items.begin(); item != items.end();)
	{
		// ������߱��Ե�
		if ((*item)->speed.y > 0 &&
			(*item)->get_geometry().y >= paddle.get_geometry().y - (*item)->get_geometry().h &&
			(*item)->get_geometry().x + (*item)->get_geometry().w / 2 > paddle.get_geometry().x &&
			(*item)->get_geometry().x + (*item)->get_geometry().w / 2 < paddle.right() &&
			(*item)->top() <= paddle.bottom())
		{
			// �ж����ͣ�����Ч��
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
		// ���û�Ե�
		else if ((*item)->bottom() == SCREEN_HEIGHT)
			(*item)->gotten = true;
		// ɾ������ֵС�ڵ���0 ��ש��
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

