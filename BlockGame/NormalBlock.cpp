#include "NormalBlock.h"


NormalBlock::NormalBlock(void)
{
	/*geometry.w = 80;
	geometry.h = 30;*/

	health = 1;
	score = 5;
}

NormalBlock::NormalBlock(int x, int y)
{
	// û��
	//NormalBlock();
	geometry.x = x;
	geometry.y = y;
	health = 1;
	score = 5;
}


NormalBlock::~NormalBlock(void)
{
}

// ���ò���--����
void NormalBlock::set_texture(SDL_Texture **textures)
{
	this->texture = textures[NORMAL_BLOCK];
	this->textures = textures;
}