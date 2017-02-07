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
SDL_Rect GraphicItem::get_geometry()
{
	return geometry;
}

// set������Ϣ
void GraphicItem::set_geometry(int x, int y, int w, int h)
{
	geometry.x = x;
	geometry.y = y;
	geometry.w = w;
	geometry.h = h;
}

void GraphicItem::set_geometry(SDL_Rect rect)
{
	geometry = rect;
}

// setλ��
void GraphicItem::set_pos(int x, int y)
{
	geometry.x = x;
	geometry.y = y;
}

// setλ��
void GraphicItem::set_pos(POINT p)
{
	geometry.x = p.x;
	geometry.y = p.y;
}

// getλ��
POINT GraphicItem::get_pos()
{
	POINT temp;
	temp.x = geometry.x;
	temp.y = geometry.y;
	return temp;
}

void GraphicItem::update(float delta_time)
{
	geometry.x += speed.x * delta_time;
	geometry.y += speed.y * delta_time;
}


void GraphicItem::set_speed_x(int s)
{
	speed.x =s;
}
void GraphicItem::set_speed_y(int s)
{
	speed.y = s;
}

// ���ÿ��
void GraphicItem::set_width(int w)
{
	geometry.w = w;
}

// �����ĸ���������ʵ�ֱ߽���Ϣ�ļ���ȡ
int GraphicItem::get_left_edge()
{
	return geometry.x;
}
int GraphicItem::get_right_edge()
{
	return geometry.x + geometry.w;
}
int GraphicItem::get_top_edge()
{
	return geometry.y;
}
int GraphicItem::get_bottom_edge()
{
	return geometry.y+geometry.h;
}

// ��ȡͼ����е�����
int GraphicItem::get_v_mid()
{
	return (get_top_edge()+get_bottom_edge()) / 2;
}
int GraphicItem::get_h_mid()
{
	return (get_left_edge() + get_right_edge())/2;
}

