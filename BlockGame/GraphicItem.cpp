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

// 设置材质
/*void GraphicItem::set_texture(SDL_Texture **texture)
{
	this->texture = texture;
	//SDL_QueryTexture(texture, NULL, NULL, &geometry.w, &geometry.h);
}*/

// get材质
SDL_Texture* GraphicItem::get_texture()
{
	return texture;
}

// get几何信息
SDL_Rect GraphicItem::get_geometry()
{
	return geometry;
}

// set几何信息
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

// set位置
void GraphicItem::set_pos(int x, int y)
{
	geometry.x = x;
	geometry.y = y;
}

// set位置
void GraphicItem::set_pos(POINT p)
{
	geometry.x = p.x;
	geometry.y = p.y;
}

// get位置
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

// 设置宽度
void GraphicItem::set_width(int w)
{
	geometry.w = w;
}

// 以下四个函数用于实现边界信息的简便获取
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

// 获取图像的中点坐标
int GraphicItem::get_v_mid()
{
	return (get_top_edge()+get_bottom_edge()) / 2;
}
int GraphicItem::get_h_mid()
{
	return (get_left_edge() + get_right_edge())/2;
}

