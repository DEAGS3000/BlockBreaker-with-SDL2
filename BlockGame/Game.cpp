#include "Game.h"
#include <algorithm>


Game::Game(void)
{
	score = 0;
	life = 0;
	paused = false;
	// 从第一关开始
	level = 1;
}

Game::~Game(void)
{
	// 释放所有砖块
	for(list<Block*>::iterator block = blocks.begin(); block!=blocks.end(); ++block)
	{
		delete *block;
	}
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
	paddle.set_geometry(SCREEN_WIDTH/2, 680,100,10);

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
