#include "Game.h"
#include <algorithm>


Game::Game(void)
{
	score = 0;
	life = 0;
	paused = false;
	// �ӵ�һ�ؿ�ʼ
	level = 1;
}

Game::~Game(void)
{
	// �ͷ�����ש��
	for(list<Block*>::iterator block = blocks.begin(); block!=blocks.end(); ++block)
	{
		delete *block;
	}
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
	paddle.set_geometry(SCREEN_WIDTH/2, 680,100,10);

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
