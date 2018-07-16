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
Vec4f GraphicItem::get_geometry()
{
	return geometry;
}

SDL_Rect GraphicItem::get_geometry_sdl()
{
	SDL_Rect temp = { static_cast<int>(geometry.x), static_cast<int>(geometry.y), static_cast<int>(geometry.w), static_cast<int>(geometry.h) };
	return temp;
}

// set几何信息
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

// set位置
void GraphicItem::set_pos(float x, float y)
{
	geometry.x = x;
	geometry.y = y;
}

// set位置
void GraphicItem::set_pos(Vec2f p)
{
	geometry.x = p.x;
	geometry.y = p.y;
}

// get位置
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

// 设置宽度
void GraphicItem::set_width(float w)
{
	geometry.w = w;
}

// 以下四个函数用于实现边界信息的简便获取
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

// 获取图像的中点坐标
int GraphicItem::get_v_mid()
{
	return (top()+bottom()) / 2;
}
int GraphicItem::get_h_mid()
{
	return (left() + right())/2;
}

