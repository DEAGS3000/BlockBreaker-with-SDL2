#include "GraphicItem.h"


GraphicItem::GraphicItem(void)
{
	geometry.x = 0;
	geometry.y = 0;
	speed.x = 0;
	speed.y = 0;
}

GraphicItem::GraphicItem(SDL_Texture *t)
{
	texture = t;
	geometry.h = 0;
	geometry.w = 0;
	geometry.x = 0;
	geometry.y = 0;
}


GraphicItem::~GraphicItem(void)
{

}

// ���ò���
/*void GraphicItem::set_texture(SDL_Texture **texture)
{
	this->texture = texture;
	//SDL_QueryTexture(texture, NULL, NULL, &geometry.w, &geometry.h);
}*/

// get����
SDL_Texture* GraphicItem::get_texture()
{
	return texture;
}

// get������Ϣ
Vec4f GraphicItem::get_geometry()
{
	return geometry;
}

SDL_Rect GraphicItem::get_geometry_sdl()
{
	SDL_Rect temp = { static_cast<int>(geometry.x), static_cast<int>(geometry.y), static_cast<int>(geometry.w), static_cast<int>(geometry.h) };
	return temp;
}

// set������Ϣ
void GraphicItem::set_geometry(float x, float y, float w, float h)
{
	geometry.x = x;
	geometry.y = y;
	geometry.w = w;
	geometry.h = h;
}

void GraphicItem::set_geometry(Vec4f rect)
{
	geometry = rect;
}

// setλ��
void GraphicItem::set_pos(float x, float y)
{
	geometry.x = x;
	geometry.y = y;
}

// setλ��
void GraphicItem::set_pos(Vec2f p)
{
	geometry.x = p.x;
	geometry.y = p.y;
}

// getλ��
Vec2f GraphicItem::get_pos()
{
	Vec2f temp;
	temp.x = geometry.x;
	temp.y = geometry.y;
	return temp;
}

void GraphicItem::update(float delta_time)
{
	geometry.x += speed.x * delta_time;
	geometry.y += speed.y * delta_time;
}


void GraphicItem::set_speed_x(float s)
{
	speed.x =s;
}
void GraphicItem::set_speed_y(float s)
{
	speed.y = s;
}

// ���ÿ��
void GraphicItem::set_width(float w)
{
	geometry.w = w;
}

// �����ĸ���������ʵ�ֱ߽���Ϣ�ļ���ȡ
int GraphicItem::left()
{
	return geometry.x;
}
int GraphicItem::right()
{
	return geometry.x + geometry.w;
}
int GraphicItem::top()
{
	return geometry.y;
}
int GraphicItem::bottom()
{
	return geometry.y+geometry.h;
}

Vec2f GraphicItem::center()
{
	Vec2f temp = { (left() + right()) / 2, (top() + bottom()) / 2 };
	return temp;
}

// ��ȡͼ����е�����
int GraphicItem::get_v_mid()
{
	return (top()+bottom()) / 2;
}
int GraphicItem::get_h_mid()
{
	return (left() + right())/2;
}

