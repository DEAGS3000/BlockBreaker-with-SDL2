#include "Window.h"
#include <cmath>
#include <SDL.h>
#include <SDL_ttf.h>



Window::Window(void)
{
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
	// ������Ϸ
	game = new Game;
	game->new_game();
	// ��ղ����б�
	for (int i = 0; i < 30; i++)
		textures[i] = nullptr;

	// ��ʼ������
	font = nullptr;
	font = TTF_OpenFont("res/SourceSansPro-Regular.ttf", 20);
	//TTF_OpenFont("res/font.ttf", 18);
	quit = false;
	// �������ʵ�����ŷ�ŵ�����Ϸ�ϱ�ɫ�Ĺ���
	mouse_on_button = false;
}


Window::~Window(void)
{
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


// ��Ⱦͼ��
void Window::refresh()
{
	SDL_RenderPresent(renderer);
}

// ����ͼ��
void Window::update()
{
	if (game_started)
	{
		//SDL_Event event;
		// ���û����ͣ
		if (!game->paused)
		{
			// ���ͼ��
			SDL_RenderClear(renderer);

			// �������
			if (game->is_clear())
			{
				MessageBox(NULL, "����OK������һ��...", "��ϲ����!", MB_OK);
				game->level++;
				game->new_game();
				game->set_textures(textures);
				return;
			}

			// ���û��С����������������
			if (game->balls.size() == 0)
			{
				if (game->life == 0)
				{
					int choice = 0;
					choice = MessageBox(NULL, "����һ�Σ�\n��������\n����˳�", "Game Over", MB_YESNO);
					// �ж��û���ѡ��
					switch (choice)
					{
						// �û�ѡ����
					case 6:
						game->level = 1;
						game->new_game();
						game->set_textures(textures);
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
					game->reset();
					game->balls.front()->set_texture(textures);
				}

			}
			// �����ز�
			//game->set_textures(textures);



			// ����
			SDL_RenderCopy(renderer, background, NULL, NULL);

			// ���°�״̬
			game->paddle.update();
			SDL_RenderCopy(renderer, game->paddle.get_texture(), NULL, &game->paddle.get_geometry());

			// ����������״̬
			for (list<Ball*>::iterator it = game->balls.begin(); it != game->balls.end(); ++it)
			{
				if (*it)
				{
					// ���С��δ���䣬���Ű���
					if (!(*it)->is_launched())
						(*it)->set_pos(game->paddle.get_pos().x + game->paddle.get_geometry().w / 2 - (*it)->get_geometry().w / 2, game->paddle.get_pos().y - (*it)->get_geometry().h);
					// TODO: ����
					(*it)->update();
					handle_collision();
					// ���С����δ׹��
					if (!(*it)->crashed)
						(*it)->update();
				}
				SDL_RenderCopy(renderer, (*it)->get_texture(), NULL, &(*it)->get_geometry());
			}

			// ����������Ʒ״̬
			for (list<Item*>::iterator item = game->items.begin(); item != game->items.end(); ++item)
			{
				if (*item)
				{
					(*item)->update();
				}
				SDL_RenderCopy(renderer, (*item)->get_texture(), NULL, &(*item)->get_geometry());
			}

			// ������ײ
			handle_collision();

			// ����׹�ٵ�С��
			for (list<Ball*>::iterator ball = game->balls.begin(); ball != game->balls.end(); )
			{
				if ((*ball)->crashed)
				{
					delete (*ball);
					// ׹��
					ball = game->balls.erase(ball);
				}
				else
					++ball;
			}


			// ����ש�����
			for (list<Block*>::iterator block = game->blocks.begin(); block != game->blocks.end(); ++block)
			{
				if (*block)
				{
					SDL_RenderCopy(renderer, (*block)->get_texture(), NULL, &(*block)->get_geometry());
				}
			}

			// ��ȡ֡�������¶�����Ϣ
			int frame = int(((SDL_GetTicks() / 10) % 30));
			for (list<Animation*>::iterator anim = game->animations.begin(); anim != game->animations.end(); )
			{
				if (*anim)
				{
					(*anim)->update2();
					SDL_RenderCopy(renderer, (*anim)->texture, &(*anim)->source_rect, &(*anim)->target_rect);
				}
				// ɾ���������
				if ((*anim)->frame == 27)
					anim = game->animations.erase(anim);
				else
					++anim;
			}

			// ��Ⱦ�������͹ؿ���
			SDL_Rect text_rect = { 880, 700, 130, 25 };
			char temp_str[50];
			sprintf(temp_str, "LEVEL: %d LIFE: %d", game->level, game->life);
			SDL_RenderCopy(renderer, render_text(temp_str), NULL, &text_rect);
		}
		else
		{
			SDL_Rect temp = { SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 50, 100, 100 };
			SDL_RenderCopy(renderer, textures[PAUSE], NULL, &temp);
		}
	}
	// ���û�п�ʼ��Ϸ
	else
	{
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
	}
}


// ��������ͼƬ��Դ
void Window::init()
{
	textures[BACKGROUND] = IMG_LoadTexture(renderer, "res/bg2.jpg");

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
	textures[ANIMATION] = load_image("res/animation2.png");
	// ��ͣ
	textures[PAUSE] = load_image("res/pause.png");
	// ����
	textures[TITLE] = load_image("res/title.jpg");
	textures[NEW_GAME] = load_image("res/new_game.png");
	textures[NEW_GAME_2] = load_image("res/new_game2.png");
	textures[CHOOSE_STAGE] = load_image("res/choose_stage.png");

	this->background = textures[BACKGROUND];

	// Ϊ��Ϸ�еĶ������ò���
	game->set_textures(textures);
}


// �����ײ
void Window::handle_collision()
{
	// ����Ѿ�û��С���ˣ�������
	if (game->balls.size() == 0)
		return;
	for (list<Ball*>::iterator ball = game->balls.begin(); ball != game->balls.end(); )
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
		if ((*ball)->speed.y > 0 && (*ball)->get_geometry().y >= game->paddle.get_geometry().y - (*ball)->get_geometry().h && (*ball)->get_geometry().x + (*ball)->get_geometry().w / 2 > game->paddle.get_geometry().x && (*ball)->get_geometry().x + (*ball)->get_geometry().w / 2 < game->paddle.get_geometry().x + game->paddle.get_geometry().w)
		{
			(*ball)->speed.y *= -1;
			// ����ٶ�Ӱ��С����ٶ�
			(*ball)->speed.x += game->paddle.speed.x / 3;
			/*// С��ˮƽ�ٶ��������12
			if(abs((*ball)->speed.x)>12)
			(*ball)->speed.x = (*ball)->speed.x / abs((*ball)->speed.x) *12;*/
			// ���ڰ�Ĳ�ͬλ�û��С��ˮƽ�ٶ���ɲ�ͬ��Ӱ��
			/*(*ball)->speed.x += ((*ball)->get_pos().x-game->stick.get_pos().x - game->stick.get_geometry().w / 2)
			/ (game->stick.get_geometry().w/2) * 12;*/

			(*ball)->speed.x = (double)((*ball)->get_pos().x + (*ball)->get_geometry().w / 2 - game->paddle.get_pos().x - game->paddle.get_geometry().w / 2)
				/ (double)(game->paddle.get_geometry().w / 2) * 8;
			// С��ˮƽ�ٶ��������12
			/*if(abs((*ball)->speed.x)>12)
			(*ball)->speed.x = (*ball)->speed.x / abs((*ball)->speed.x) *12;*/
		}

		// ש��
		bool hit = false;
		for (list<Block*>::iterator block = game->blocks.begin(); block != game->blocks.end();)
		{

			hit = false;
			if (*block)
			{
				// ����
				/*if ((*block)->get_geometry().x == 287 && (*block)->get_geometry().y == 370 && (*ball)->get_top_edge() <= 370 && (*ball)->speed.y < 0)
				{
					int a;
					a = 10;
				}*/

				// ��ײש�����±߽�
				// ���С��������λ��ש�����ұ߽�֮��
				// ���ڽǶȣ��Ȱѵ���Ҳ������ײ���±߽���
				double algle_with_ball = (*block)->angle_with_ball((*ball)->get_geometry());
				double right_top_angle = (*block)->right_top_angle();
				double left_top_angle = (*block)->left_top_angle();
				// �ж�С���Ƿ���ש�鷢���˽Ӵ�
				if (((*ball)->get_h_mid() >= (*block)->get_left_edge() && (*ball)->get_h_mid() <= (*block)->get_right_edge())
					&& (((*block)->angle_with_ball((*ball)->get_geometry()) <= (*block)->right_top_angle()
						&& (*block)->angle_with_ball((*ball)->get_geometry()) >= (*block)->left_top_angle())
						|| ((*block)->angle_with_ball((*ball)->get_geometry()) <= (*block)->left_bottom_angle()
							&& (*block)->angle_with_ball((*ball)->get_geometry()) >= (*block)->right_bottom_angle())))
				{
					// ��ײש���½�
					if ((*ball)->get_top_edge() < (*block)->get_bottom_edge() && (*ball)->get_bottom_edge() > (*block)->get_bottom_edge())
					{
						(*ball)->hit(BOTTOM);
						(*block)->hit_by((*ball), &game->items);
					}

					// ��ײש���Ͻ�
					else if ((*ball)->get_bottom_edge() > (*block)->get_top_edge() && (*ball)->get_top_edge() < (*block)->get_top_edge())
					{
						(*ball)->hit(TOP);
						(*block)->hit_by((*ball), &game->items);
					}
				}
				// ��ײש�����ұ߽�
				// ���С��������λ��ש�����±߽�֮��
				else if ((*ball)->get_v_mid() >= (*block)->get_top_edge() && (*ball)->get_v_mid() <= (*block)->get_bottom_edge())
				{

					// ��ײש�����
					int ball_right = (*ball)->get_right_edge();
					int block_left = (*block)->get_left_edge();
					int ball_left = (*ball)->get_left_edge();

					if ((*ball)->get_right_edge() > (*block)->get_left_edge() && (*ball)->get_left_edge() < (*block)->get_left_edge())
					{
						(*ball)->hit(LEFT);
						(*block)->hit_by((*ball), &game->items);
					}
					// ��ײש���ҽ�
					else if ((*ball)->get_left_edge() < (*block)->get_right_edge() && (*ball)->get_right_edge() > (*block)->get_right_edge())
					{
						(*ball)->hit(RIGHT);
						(*block)->hit_by((*ball), &game->items);
					}
				}
				// ɾ������ֵС�ڵ���0 ��ש��
				// ���ש�鱻�ƻ�������һ������
				if ((*block)->health <= 0)
				{
					// ��ש���λ�ô���һ������
					game->animations.push_back(new Animation(textures[ANIMATION],
						(*block)->get_h_mid() - 35,
						(*block)->get_v_mid() - 17));
					delete (*block);
					*block = nullptr;
					block = game->blocks.erase(block);
					hit = true;
				}
				if (!hit)
					block++;
			}
		}
		++ball;
	}
	// ����
	for (list<Item*>::iterator item = game->items.begin(); item != game->items.end();)
	{
		// ������߱��Ե�
		if ((*item)->speed.y > 0 &&
			(*item)->get_geometry().y >= game->paddle.get_geometry().y - (*item)->get_geometry().h &&
			(*item)->get_geometry().x + (*item)->get_geometry().w / 2 > game->paddle.get_geometry().x &&
			(*item)->get_geometry().x + (*item)->get_geometry().w / 2 < game->paddle.get_right_edge() &&
			(*item)->get_top_edge() <= game->paddle.get_bottom_edge())
		{
			// �ж����ͣ�����Ч��
			switch ((*item)->type)
			{
			case ITEM_LONG_PADDLE:
				game->paddle.size_up();
				(*item)->gotten = true;
				break;
			case ITEM_IMPROVE_BALL:
				game->improve_ball();
				(*item)->gotten = true;
				break;
			case ITEM_TRIPLE_BALL:
				game->triple_ball();
				(*item)->gotten = true;
				break;
			case ITEM_ADD_LIFE:
				game->life++;
				(*item)->gotten = true;
				break;
			}
		}
		// ���û�Ե�
		else if ((*item)->get_bottom_edge() == SCREEN_HEIGHT)
			(*item)->gotten = true;
		// ɾ������ֵС�ڵ���0 ��ש��
		if ((*item)->gotten)
		{
			delete (*item);
			*item = nullptr;
			item = game->items.erase(item);
			continue;
		}
		++item;
	}
}

// ��ȡͼ��
SDL_Texture* Window::load_image(string path)
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

SDL_Texture* Window::render_text(string message)
{
	SDL_Color color = { 255, 0, 0 };
	SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
	//Clean up unneeded stuff
	SDL_FreeSurface(surf);


	return texture;
}